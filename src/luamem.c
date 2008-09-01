#include "lcommon.h"

#define BUFF_INITIALSIZE	128

#define MEMHANDLE	"luamem*"
#define MEMBUFHANDLE	"luamembuf*"

#define mem_reset(mp)	(mp)->t = (mp)->ot

LUA_API void lua_pushtable (lua_State *L, const void *t) {
  lua_lock(L);
  sethvalue(L->top, t);
  api_incr_top(L);
  lua_unlock(L);
}

/*
 * Returns: mem_udata
 */
static void *
mem_newpointer (lua_State *L, size_t n)
{
    luamem_t *mp;

    n += sizeof (luamem_t);
    mp = lua_newuserdata (L, n);
    memset (mp, 0, n);
    mp->t.size = mp->ot.size = sizeof (char);
    mp->t.flags = mp->ot.flags = LUAMEM_ARRAY;

    luaL_getmetatable (L, MEMHANDLE);
    lua_setmetatable (L, -2);
    return mp;
}

/*
 * Arguments: count (number), [zero_fill (boolean)]
 * Returns: [mem_udata]
 */
static int
mem_alloc (lua_State *L)
{
    size_t n = luaL_optlong (L, 1, sizeof (size_t));
    luamem_t *mp = mem_newpointer (L, 0);

    mp->flags = LUAMEM_ALLOC;
    mp->len = n;
    mp->data = lua_toboolean (L, 2) ? calloc (n, 1) : malloc (n);
    return mp->data ? 1 : 0;
}

/*
 * Arguments: mem_udata, count (number)
 * Returns: [mem_udata]
 */
static int
mem_realloc (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    size_t n = (long) lua_tonumber (L, 2);
    void *p;

    if (!(p = realloc (mp->data, n)))
	return 0;
    mp->len = n;
    mp->data = p;
    lua_settop (L, 1);
    return 1;
}

#ifdef LUAMEM_USE_MAP
/*
 * Arguments: fd_udata, [protection (string: "rw"),
 *	offset (number), length (number), private/shared (boolean)]
 * Returns: [mem_udata]
 */
static int
mem_map (lua_State *L)
{
    const char *ptr = lua_touserdata (L, 1);
    int fd = (ptr) ? *((int *) ptr) : -1;  /* named or anonymous mapping */
    off_t off = (off_t) lua_tonumber (L, 3);
    size_t len = (long) lua_tonumber (L, 4);
    int prot = 0, flags;
    luamem_t *mp;

#ifndef WIN32
#ifndef MAP_ANON
    int is_anon = 0;
#endif
    /* length */
    if (!len) {
	struct stat sb;
	if (fd == -1 || fstat (fd, &sb) < 0)
	    return 0;
	len = sb.st_size - off;
    }
    /* protection and flags */
    prot = PROT_READ;
    ptr = lua_tostring (L, 2);
    if (ptr) {
	if (ptr[0] == 'w')
	    prot = PROT_WRITE;
	else if (ptr[1] == 'w')
	    prot |= PROT_WRITE;
    }
    flags = (lua_isboolean (L, -1) && lua_toboolean (L, -1))
     ? MAP_PRIVATE : MAP_SHARED;
    /* named or anonymous shared memory */
    if (fd == -1) {
#ifdef MAP_ANON
	flags |= MAP_ANON;
#else
	if ((fd = open ("/dev/zero", O_RDWR)) == -1)
	    return 0;
	is_anon = 1;
#endif
    }
    /* map file to memory */
    ptr = mmap (0, len, prot, flags, (int) fd, off);
#ifndef MAP_ANON
    if (is_anon) close (fd);
#endif
    if (ptr == MAP_FAILED) return 0;

#else
    /* length */
    if (!len) {
	if (fd == -1) return 0;
	len = GetFileSize ((HANDLE) fd, NULL);
	if (len == INVALID_FILE_SIZE)
	    return 0;
	len -= off;
    }
    /* protection and flags */
    ptr = lua_tostring (L, 2);
    if (ptr && (ptr[0] == 'w' || ptr[1] == 'w')) {
	prot = PAGE_READWRITE;
	flags = FILE_MAP_WRITE;
    } else {
	prot = PAGE_READONLY;
	flags = FILE_MAP_READ;
    }
    if (lua_isboolean (L, -1) && lua_toboolean (L, -1)) {
	prot = PAGE_WRITECOPY;
	flags = FILE_MAP_COPY;
    }
    /* map file to memory */
    {
	HANDLE hMap = CreateFileMapping ((HANDLE) fd, NULL, prot, 0, len, NULL);
	if (!hMap) return 0;
	ptr = MapViewOfFile (hMap, flags, 0, off, len);
	CloseHandle (hMap);
	if (!ptr) return 0;
    }
#endif

    mp = mem_newpointer (L, 0);
    mp->flags = LUAMEM_MAP;
    mp->data = (void *) ptr;
    mp->len = len;
    return 1;
}

/*
 * Arguments: mem_udata
 * Returns: [mem_udata]
 */
static int
mem_sync (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);

#ifndef WIN32
    return !msync (mp->data, mp->len, MS_SYNC) ? 1 : 0;
#else
    return FlushViewOfFile (mp->data, 0) ? 1 : 0;
#endif
}
#endif /* LUAMEM_USE_MAP */

/*
 * Arguments: [count (number)]
 * Returns: [membuf_udata]
 */
static int
mem_buffer (lua_State *L)
{
    size_t n = luaL_optlong (L, 1, BUFF_INITIALSIZE);
    luamem_t *mp = mem_newpointer (L,
     sizeof (luamembuf_t) - sizeof (luamem_t));

    luaL_getmetatable (L, MEMBUFHANDLE);
    mp->t.meta = mp->ot.meta = lua_topointer (L, -1);
    lua_pop (L, 1);

    mp->flags = LUAMEM_ALLOC;
    mp->len = n;
    mp->data = malloc (n);
    return mp->data ? 1 : 0;
}

/*
 * Arguments: mem_udata
 * Returns: mem_udata
 */
static int
mem_free (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);

    if (mp->data) {
	switch (mp->flags & LUAMEM_FIXED) {
	case LUAMEM_ALLOC:
	    free (mp->data);
	    break;
#ifdef LUAMEM_USE_MAP
	case LUAMEM_MAP:
#ifndef WIN32
	    munmap (mp->data, mp->len);
#else
	    UnmapViewOfFile (mp->data);
#endif
	    break;
#endif /* LUAMEM_USE_MAP */
	}
	mp->data = NULL;
    }
    return 1;
}


/*
 * Arguments: ..., mem_udata
 */
static void
mem_setweak (lua_State *L, const void *meta)
{
    if (meta) {
	/* weak-table: udata => metatable */
	lua_pushlightuserdata (L, (void *) luaopen_mem);
	lua_rawget (L, LUA_REGISTRYINDEX);
	lua_pushvalue (L, -2);
	lua_pushtable (L, meta);
	lua_rawset (L, -3);
	lua_pop (L, 1);
    }
}

/*
 * Arguments: ..., mem_udata, type_udata
 * Returns: mem_udata
 */
static int
mem_def (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, -2);
    luamem_ident_t *tp = lua_touserdata (L, -1);

    mp->t = mp->ot = *tp;
    lua_pop (L, 1);
    mem_setweak (L, tp->meta);
    return 1;
}

/*
 * Arguments: type_udata
 * Returns: [mem_udata]
 */
static int
mem_new (lua_State *L)
{
    luamem_ident_t *tp = lua_touserdata (L, 1);
    luamem_t *mp = mem_newpointer (L, tp->size);

    mp->len = tp->size;
    mp->data = (void *) (mp + 1);
    lua_pushvalue (L, 1);
    return mem_def (L);
}

/*
 * Arguments: mem_udata (source) | ludata | number, [mem_udata]
 * Returns: mem_udata
 */
static int
mem_base (lua_State *L)
{
    luamem_t *src = lua_touserdata (L, 1);
    luamem_t *mp = lua_touserdata (L, 2);

    if (!mp)
	mp = mem_newpointer (L, 0);
    else if (mp->flags & LUAMEM_FIXED)
	luaL_argerror (L, 2, "memory is fixed");
    if (src) {
	if (lua_type (L, 1) == LUA_TUSERDATA) {
	    *mp = *src;
	    mp->flags = 0;
	    mp->data += mp->t.off;
	    mp->t.off = 0;
	    mp->ot = mp->t;
	    mem_setweak (L, mp->t.meta);
	    return 1;
	}
    } else
	src = (void *) (long) lua_tonumber (L, 1);
    mp->data = (void *) src;
    return 1;
}

/*
 * Arguments: mem_udata, [length (number)]
 * Returns: mem_udata | length (number)
 */
static int
mem_length (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);

    if (lua_gettop (L) > 1) {
	if (mp->flags & LUAMEM_MAP)
	    luaL_argerror (L, 2, "memory is mapped");
	mp->len = (long) lua_tonumber (L, 2);
	lua_settop (L, 1);
    } else
	lua_pushnumber (L, mp->len);
    return 1;
}

/*
 * Arguments: mem_udata, mem_udata (source), [count (number)]
 * Returns: [mem_udata]
 */
static int
mem_copy (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    luamem_t *src = lua_touserdata (L, 2);
    size_t n = luaL_optlong (L, 3, mp->t.size);

    lua_settop (L, 1);
    return memcpy (luamem_ptr (mp), src->data + src->t.off, n) ? 1 : 0;
}

/*
 * Arguments: mem_udata, [byte (number), count (number)]
 * Returns: [mem_udata]
 */
static int
mem_set (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    int ch = (int) lua_tonumber (L, 2);
    size_t n = luaL_optlong (L, 3, mp->t.size);

    lua_settop (L, 1);
    return memset (luamem_ptr (mp), ch, n) ? 1 : 0;
}

/*
 * Arguments: mem_udata, [byte (number), size (number)]
 * Returns: [index (number)]
 */
static int
mem_char (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    int ch = (int) lua_tonumber (L, 2);
    size_t n = luaL_optlong (L, 3, mp->t.size);
    const char *p, *base;

    base = luamem_ptr (mp);
    p = memchr(base, ch, n);
    if (p) {
	lua_pushnumber (L, p - base);
	return 1;
    }
    return 0;
}

/*
 * Arguments: mem_udata | type_udata
 * Returns: number
 */
static int
mem_sizeof (lua_State *L)
{
    luamem_ident_t *tp = lua_touserdata (L, 1);
    lua_pushnumber (L, tp->size);
    return 1;
}

/*
 * Arguments: mem_udata | addr (number), [length (number)]
 * Returns: string
 */
static int
mem_tostring (lua_State *L)
{
    void *ptr;
    size_t len = 0;

    if (lua_isuserdata (L, 1)) {
	luamem_t *mp = lua_touserdata (L, 1);
	ptr = luamem_ptr (mp);
	len = mp->t.size;
    } else
	ptr = (void *) (size_t) lua_tonumber (L, 1);
    lua_pushlstring (L, ptr, luaL_optlong (L, 2, len > 1 ? len : strlen (ptr)));
    return 1;
}

/*
 * Arguments: struct (table), [alignment (number), is_function (boolean)]
 * Returns: type_udata
 */
static int
mem_struct (lua_State *L)
{
    int align = luaL_optint (L, 2, sizeof (long));
    int is_function = lua_toboolean (L, 3);
    luamem_ident_t *newtp, *prevtp, *settp = NULL;
    int offset = 0, size = 0, idx = 0;
    int cnt, i;

    lua_settop (L, 1);
    newtp = lua_newuserdata (L, sizeof (luamem_ident_t));
    /* have not members? */
    if (!lua_istable (L, 1)) {
	i = -1;
	goto end;
    }
    lua_newtable (L);  /* metatable */

    /* traverse members of struct */
    for (prevtp = NULL, cnt = 0, i = 1; i != 0; ++i) {
	lua_rawgeti (L, 1, i);
	switch (lua_type (L, -1)) {
	case LUA_TNIL: i = -1; break;
	case LUA_TFUNCTION:	/* lua-function: name required */
	    lua_rawgeti (L, 1, ++i);
	    lua_pushvalue (L, -2);
	    lua_rawset (L, 3);
	    break;
	case LUA_TNUMBER:	/* alignment | array size */
	    cnt = (int) lua_tonumber (L, -1);
	    break;
	case LUA_TUSERDATA:
	    settp = lua_touserdata (L, -1);
	    size = (settp->flags & LUAMEM_TCFUNCTION)
	     ? (int) sizeof (void *) : settp->size;
	    /* memory alignment */
	    if (size > 0 && prevtp
	     && (cnt > 0 || (cnt >= 0 && align > 0))) {
		int pad = prevtp->size % (cnt > 0 ? size * cnt : align);
		if (pad > 0) offset += size - pad;
	    }
	    cnt = 0;
	    break;
	default:
	    {
		luamem_ident_t *tp = lua_newuserdata (L, sizeof (luamem_ident_t));
		int off;

		prevtp = tp;
		*tp = *settp;
		tp->size = size;
		off = size * (abs (cnt) > 1 ? abs (cnt) : 1);
		if (cnt >= 0) {
		    tp->off = offset;
		    offset += off;
		} else {  /* union */
		    tp->off = 0;
		    if (off > offset) offset = off;
		}
		if (cnt != 0)
		    tp->flags |= LUAMEM_ARRAY;
		if (tp->meta) {
		    lua_pushtable (L, tp->meta);
		    lua_setmetatable (L, -2);
		}
		/* set index (1..) => type_udata */
		lua_pushvalue (L, -1);
		lua_rawseti (L, 3, ++idx);
		/* set field => type_udata */
		lua_rawset (L, 3);
		cnt = 0;
		continue;
	    }
	}
	lua_pop (L, 1);  /* pop value */
    }
end:
    if (is_function) {
	newtp->flags = LUAMEM_TCFUNCTION | ((i >= 0) ? LUAMEM_STRUCT : 0);
	newtp->size = sizeof (void *);
    } else {
	newtp->flags = (idx > 0) ? LUAMEM_STRUCT : 0;
	newtp->size = !offset ? (int) sizeof (char) : offset;
    }
    newtp->off = 0;
    if (i >= 0) {
	/* set size of arguments buffer of function */
	if (is_function) {
	    lua_pushnumber (L, offset);
	    lua_rawseti (L, 3, 0);
	}
	newtp->meta = lua_topointer (L, 3);
	lua_setmetatable (L, 2);
    } else newtp->meta = NULL;
    return 1;
}

/*
 * Arguments: struct (table)
 * Returns: type_udata
 */
static int
mem_cfunction (lua_State *L)
{
    lua_settop (L, 1);
    lua_pushnil (L);  /* standard alignment */
    lua_pushboolean (L, 1);
    return mem_struct (L);
}


static int
mem_getaddr (lua_State *L, luamem_t *mp, int off)
{
    lua_pushnumber (L, (long) mp->data + off);
    mp->flags &= ~LUAMEM_ADDR;
    mem_reset (mp);
    return 1;
}

/*
 * Arguments: mem_udata, ..., [count (number)]
 * Returns: mem_udata
 */
static int
mem_add (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    return mem_getaddr (L, mp, mp->t.size * luaL_optint (L, -1, 1));
}

/*
 * Arguments: mem_udata, [count (number)]
 * Returns: mem_udata
 */
static int
mem_sub (lua_State *L)
{
    lua_pushnumber (L, - luaL_optlong (L, 2, 1));
    return mem_add (L);
}


#define LUAMEM_CMP_EQ	0
#define LUAMEM_CMP_LT	1
#define LUAMEM_CMP_LE	2

/*
 * Arguments: mem_udata1, mem_udata2
 * Returns: boolean
 */
static int
mem_cmp (lua_State *L, int req)
{
    luamem_t *mp1 = lua_touserdata (L, 1);
    luamem_t *mp2 = lua_touserdata (L, 2);

    if (mp2) {
	int cmp = luamem_ptr (mp2) - luamem_ptr (mp1);
	lua_pushboolean (L,
	 (req == LUAMEM_CMP_EQ && cmp == 0)
	 || (req == LUAMEM_CMP_LT && cmp > 0)
	 || (req == LUAMEM_CMP_LE && cmp >= 0));
	return 1;
    }
    return 0;
}

static int
mem_eq (lua_State *L)
{
    return mem_cmp (L, LUAMEM_CMP_EQ);
}

static int
mem_lt (lua_State *L)
{
    return mem_cmp (L, LUAMEM_CMP_LT);
}

static int
mem_le (lua_State *L)
{
    return mem_cmp (L, LUAMEM_CMP_LE);
}


static int
mem_getvalue (lua_State *L, luamem_t *mp, char *ptr, int type)
{
    switch (type) {
    case LUAMEM_TBYTE:
	lua_pushnumber (L, *((unsigned char *) ptr));
	break;
    case LUAMEM_TSHORT:
	lua_pushnumber (L, *((unsigned short *) ptr));
	break;
    case LUAMEM_TINT:
	lua_pushnumber (L, *((int *) ptr));
	break;
    case LUAMEM_TLONG:
	lua_pushnumber (L, *((long *) ptr));
	break;
    case LUAMEM_TSINGLE:
	lua_pushnumber (L, *((float *) ptr));
	break;
    case LUAMEM_TDOUBLE:
	lua_pushnumber (L, *((double *) ptr));
	break;
    case LUAMEM_TNUMBER:
	lua_pushnumber (L, *((lua_Number *) ptr));
	break;
    case LUAMEM_TPSTR:
	lua_pushstring (L, *((char **) ptr));
	break;
    case LUAMEM_TPSTRLEN:
	lua_pushlstring (L,
	 ((struct pstrlen *) ptr)->s, ((struct pstrlen *) ptr)->len);
	break;
    case LUAMEM_TREF:
    case LUAMEM_THANDLE:
	lua_pushnumber (L, (long) *((void **) ptr));
	break;
    case LUAMEM_TBITSTRING:
	{
	    int idx = (int) lua_tonumber (L, 2);
	    ptr += (idx >> 3) - idx;  /* correct address */
	    idx = 1 << (idx & 7);  /* bit */
	    lua_pushboolean (L, *ptr & idx);
	}
	break;
    }
    mem_reset (mp);
    return 1;
}

/*
 * Arguments: mem_udata, key (any)
 * Returns: mem_udata | value (any of primitive)
 */
static int
mem_index (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    int type;

    switch (lua_type (L, 2)) {
    case LUA_TNIL:
	mem_reset (mp);
	break;
    case LUA_TNUMBER:	/* index */
	if (mp->flags & LUAMEM_FIELDNO)
	    mp->flags &= ~LUAMEM_FIELDNO;
	else {
	    int off = mp->t.off + mp->t.size * (int) lua_tonumber (L, 2);
	    if (mp->flags & LUAMEM_ADDR)
		return mem_getaddr (L, mp, off);
	    if (!luamem_isstruct (&mp->t))
		return mem_getvalue (L, mp, mp->data + off,
		 luamem_type (&mp->t));
	    else mp->t.off = off;
	    break;
	}
    default:
	if (!mp->t.meta) return 0;
	lua_pushtable (L, mp->t.meta);
	lua_pushvalue (L, 2);
	lua_rawget (L, -2);
	type = lua_type (L, -1);
	if (type == LUA_TNIL || type == LUA_TFUNCTION)
	    return 1;
	else {
	    luamem_ident_t *tp = lua_touserdata (L, -1);
	    type = luamem_type (tp);

	    if (mp->flags & LUAMEM_ADDR)
		return mem_getaddr (L, mp, mp->t.off + tp->off);
	    if (!luamem_isstruct (tp) && !luamem_isarray (tp))
		return mem_getvalue (L, mp, luamem_ptr (mp) + tp->off, type);
	    else {
		int off = mp->t.off + tp->off;
		mp->t = *tp;
		mp->t.off = off;
	    }
	}
    }
    lua_settop (L, 1);
    return 1;
}

/*
 * Arguments: mem_udata, ..., key (any), value (any)
 */
static int
mem_newindex (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    luamem_ident_t *tp = &mp->t;
    char *ptr = luamem_ptr (mp);
    int lt = lua_type (L, -1);
    int indexed = 0;
    int idx = 0;  /* for bitstring */

    switch (lua_type (L, -2)) {
    case LUA_TNIL:	/* reset to origin type */
	tp = &mp->ot;
	break;
    case LUA_TNUMBER:	/* index */
	if (mp->flags & LUAMEM_FIELDNO)
	    mp->flags &= ~LUAMEM_FIELDNO;
	else {
	    idx = (int) lua_tonumber (L, -2);
	    if (mp->flags & LUAMEM_INDEXNO) {
		mp->flags &= ~LUAMEM_INDEXNO;
		--idx;
	    }
	    if (idx != 0)
		ptr += tp->size * idx;
	    indexed = 1;
	    break;
	}
    default:
	if (!tp->meta) return 0;
	lua_pushtable (L, tp->meta);
	lua_pushvalue (L, -3);  /* key */
	lua_rawget (L, -2);  /* type_udata */
	if (lua_isnil (L, -1))
	    luaL_argerror (L, -1, "invalid key");
	tp = lua_touserdata (L, -1);
	if (!tp) {
	    lua_pushvalue (L, -4);  /* name */
	    lua_pushvalue (L, -4);  /* lua-function */
	    lua_rawset (L, -4);
	    goto end;
	}
	lua_pop (L, 2);
    }
    if (!indexed)
	ptr += tp->off;
    if (lt == LUA_TFUNCTION)
	*((void **) ptr) = (void *) lua_tocfunction (L, -1);
    else if (luamem_isstruct (tp)) {
	if (lt == LUA_TTABLE) {  /* multiple values */
	    void *data = mp->data;
	    luamem_ident_t settp = *tp;

	    settp.off = 0;
	    indexed = luamem_isarray (tp) && !indexed;
	    /* traverse values */
	    lua_pushnil (L);
	    while (lua_next (L, -2)) {
		if (lua_type (L, -2) == LUA_TNUMBER)
		    mp->flags |= indexed ? LUAMEM_INDEXNO : LUAMEM_FIELDNO;
		mp->t = settp;
		mp->data = ptr;
		mem_newindex (L);
		lua_pop (L, 1);  /* pop value */
	    }
	    mp->data = data;
	} else {  /* copy struct */
	    luamem_t *src = lua_touserdata (L, -1);

	    if (!src || tp->size != src->t.size)
		luaL_argerror (L, -1, "value is not same struct");
	    memcpy (ptr, luamem_ptr (src), src->t.size);
	    mem_reset (src);
	}
    } else {
	int type = luamem_type (tp);
	if (lt == LUA_TNUMBER) {
	    lua_Number num = lua_tonumber (L, -1);

	    switch (type) {
	    case LUAMEM_TBYTE:
		*((unsigned char *) ptr) = (unsigned char) num;
		break;
	    case LUAMEM_TSHORT:
		*((unsigned short *) ptr) = (unsigned short) num;
		break;
	    case LUAMEM_TINT:
		*((int *) ptr) = (int) num;
		break;
	    case LUAMEM_TLONG:
		*((long *) ptr) = (long) num;
		break;
	    case LUAMEM_TSINGLE:
		*((float *) ptr) = (float) num;
		break;
	    case LUAMEM_TDOUBLE:
		*((double *) ptr) = (double) num;
		break;
	    case LUAMEM_TNUMBER:
		*((lua_Number *) ptr) = num;
		break;
	    case LUAMEM_TREF:
	    case LUAMEM_THANDLE:
		*((void **) ptr) = (void *) (long) num;
		break;
	    case LUAMEM_TBITSTRING:
		ptr += (idx >> 3) - idx;  /* correct address */
		idx = 1 << (idx & 7);  /* bit */
		if (num == 0) *ptr &= ~idx;	/* clear */
		else if (num > 0) *ptr |= idx;	/* set */
		else *ptr ^= idx;		/* invert */
		break;
	    }
	} else if (type == LUAMEM_TREF)
	    *((void **) ptr) = lua_touserdata (L, -1);
	else if (type == LUAMEM_THANDLE)
	    *((void **) ptr) = lua_unboxpointer (L, -1);
	else {
	    const char *s = lua_tostring (L, -1);
	    size_t len = lua_strlen (L, -1);

	    switch (type) {
	    case LUAMEM_TPSTR:
		*((const char **) ptr) = s;
		break;
	    case LUAMEM_TPSTRLEN:
		((struct pstrlen *) ptr)->s = s;
		((struct pstrlen *) ptr)->len = len;
		break;
	    default:
		memcpy (ptr, s, len);
	    }
	}
    }
end:
    mem_reset (mp);
    return 0;
}


struct arr32 {char dummy[32];};
struct arr64 {char dummy[64];};
typedef long (*luamem_arr32_fn) (struct arr32);
typedef long (*luamem_arr64_fn) (struct arr64);

/*
 * Arguments: mem_udata (cfunction),
 *	arguments (table) | address (mem_udata | number), [length (number)]
 * Returns: number
 */
static long
mem_call_arg (lua_State *L, luamem_t *mp, luamem_arr64_fn fn)
{
    int len;
    void *args;
    struct arr64 buf;

    switch (lua_type (L, 2)) {
    case LUA_TTABLE:	/* arguments via table */
	/* get size of arguments buffer of function */
	lua_pushtable (L, mp->t.meta);
	lua_rawgeti (L, -1, 0);
	len = (int) lua_tonumber (L, -1);
	lua_pop (L, 2);
	if (len > 64) goto err;

	args = &buf;
	lua_pushnumber (L, 0);
	lua_pushvalue (L, 2);
	/* fill arguments buffer */
	{
	    void *data = mp->data;
	    mp->data = args;
	    mp->t.off = 0;
	    memset (args, 0, len);  /* default values of arguments is zero */
	    mem_newindex (L);
	    mp->data = data;
	}
	break;
    case LUA_TUSERDATA:	/* mem_udata as address of arguments buffer */
	mp = lua_touserdata (L, -1);
	len = mp->t.size;
	if (len > 64) goto err;
	args = luamem_ptr (mp);
	mem_reset (mp);
	break;
    case LUA_TNUMBER:	/* number as address of arguments buffer */
	len = luaL_optint (L, 3, sizeof (long));
	if (len > 64) goto err;
	args = (void *) (long) lua_tonumber (L, 2);
	break;
    default: return 0;
    }
    if (len <= 32)
	return ((luamem_arr32_fn) fn) (*((struct arr32 *) args));
    return fn (*((struct arr64 *) args));
err:
    return luaL_argerror (L, 2, "Too much arguments");
}

/*
 * Arguments: mem_udata, option (string), [arguments ...]
 * Returns: mem_udata
 * |
 * Arguments: mem_udata (cfunction), [arguments (table)]
 * Returns: number
 */
static int
mem_call (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);

    if (luamem_type (&mp->t) == LUAMEM_TCFUNCTION) {
	void *fn = *((void **) luamem_ptr (mp));

	if (!fn) return 0;
	lua_pushnumber (L, (lua_gettop (L) == 1)
	 ? ((long (*) ()) fn) () : mem_call_arg (L, mp, (luamem_arr64_fn) fn));
    } else {
	const char *s = lua_tostring (L, 2);
	if (s)
	    switch (s[0]) {
	    case '=':	/* set base address and length */
		if (lua_type (L, 3) == LUA_TLIGHTUSERDATA)
		    s = (void *) lua_touserdata (L, 3);
		else
		    s = (void *) (long) lua_tonumber (L, 3);
		mp->data = (void *) s;
		if (!lua_isnil (L, 4))
		    mp->len = (long) lua_tonumber (L, 4);
		break;
	    case 'f':	/* get field by number */
		mp->flags |= LUAMEM_FIELDNO;
		break;
	    case '&':	/* get address */
		if (luamem_isstruct (&mp->t) || luamem_isarray (&mp->t))
		    mp->flags |= LUAMEM_ADDR;  /* of next field */
		else
		    return mem_getaddr (L, mp, mp->t.off);
		break;
	    case '+':	/* increment */
	    case '-':	/* decrement */
		if (mp->flags & LUAMEM_FIXED)
		    luaL_argerror (L, 2, "memory is fixed");
		{
		    int off = lua_isnil (L, 3) ? mp->t.size
		     : (long) lua_tonumber (L, 3);
		    mp->data += (s[0] == '+') ? off : - off;
		}
		break;
	    }
	lua_settop (L, 1);
    }
    return 1;
}

/*
 * Arguments: mem_udata
 * Returns: string
 */
static int
mem_info (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);
    char buf[32];

    sprintf (buf, "mem (%p)", luamem_ptr (mp));
    lua_pushstring (L, buf);
    return 1;
}

/*
 * Arguments: mem_udata, [writable (boolean)]
 * Returns: address (number), length (number)
 */
static int
mem_tobuffer (lua_State *L)
{
    luamem_t *mp = lua_touserdata (L, 1);

    lua_pushnumber (L, (long) luamem_ptr (mp));
    lua_pushnumber (L, mp->len - mp->ot.off);
    return 2;
}


extern luaL_reg membuf_meth[];

static luaL_reg mem_meth[] = {
    {"__gc",		mem_free},
    {"__add",		mem_add},
    {"__sub",		mem_sub},
    {"__eq",		mem_eq},
    {"__lt",		mem_lt},
    {"__le",		mem_le},
    {"__index",		mem_index},
    {"__newindex",	mem_newindex},
    {"__call",		mem_call},
    {"__tostring",	mem_info},
    {"__tobuffer",	mem_tobuffer},
    {NULL, NULL}
};

static luaL_reg memlib[] = {
    {"alloc",		mem_alloc},
    {"realloc",		mem_realloc},
#ifdef LUAMEM_USE_MAP
    {"map",		mem_map},
    {"sync",		mem_sync},
#endif
    {"buffer",		mem_buffer},
    {"free",		mem_free},
    {"def",		mem_def},
    {"new",		mem_new},
    {"length",		mem_length},
    {"base",		mem_base},
    {"copy",		mem_copy},
    {"set",		mem_set},
    {"char",		mem_char},
    {"sizeof",		mem_sizeof},
    {"tostring",	mem_tostring},
    {"struct",		mem_struct},
    {"cfunction",	mem_cfunction},
    {NULL, NULL}
};

/*
 * Arguments: mem_lib (table)
 */
static void
createmeta (lua_State *L)
{
    /* Same order as defined LUAMEM_T* */
    static struct meta_s {
	const char *name;
	int size;
    } meta[] = {
	{"byte",	sizeof (char)},
	{"short",	sizeof (short)},
	{"int",		sizeof (int)},
	{"long",	sizeof (long)},
	{"single",	sizeof (float)},
	{"double",	sizeof (double)},
	{"number",	sizeof (lua_Number)},
	{"pstr",	sizeof (char *)},
	{"pstrlen",	sizeof (struct pstrlen)},
	{"ref",		sizeof (void *)},
	{"handle",	sizeof (void *)},
	{"bitstring",	sizeof (char)},
    };
    int i;

    for (i = 0; i < (int) (sizeof (meta) / sizeof (struct meta_s)); ++i) {
	luamem_ident_t *tp;

	lua_pushstring (L, meta[i].name);
	tp = lua_newuserdata (L, sizeof (luamem_ident_t));
	lua_rawset (L, -3);
	tp->size = meta[i].size;
	tp->off = 0;
	tp->meta = NULL;
	tp->flags = i;  /* LUAMEM_T* */
    }

    /* Create weak-table in the registry */
    lua_pushlightuserdata (L, (void *) luaopen_mem);
    lua_newtable (L);
    lua_pushliteral (L, "__mode");
    lua_pushliteral (L, "k");  /* weak keys */
    lua_rawset (L, -3);  /* metatable.__mode = "k" */
    lua_rawset (L, LUA_REGISTRYINDEX);

    luaL_newmetatable (L, MEMHANDLE);
    luaL_openlib (L, NULL, mem_meth, 0);

    luaL_newmetatable (L, MEMBUFHANDLE);
    lua_newtable (L);  /* upvalue for input buffer */
    lua_newtable (L);  /* upvalue for output buffer */
    lua_pushliteral (L, "__mode");
    lua_pushliteral (L, "k");  /* weak keys */
    lua_pushvalue (L, -2);
    lua_pushvalue (L, -2);
    lua_rawset (L, -5);
    lua_rawset (L, -3);
    luaL_openlib (L, NULL, membuf_meth, 2);
}

/*
 * Open mem library
 */
LUALIB_API int
luaopen_mem (lua_State *L)
{
    luaL_openlib (L, LUA_MEMLIBNAME, memlib, 0);
    createmeta (L);
    return 0;
}

