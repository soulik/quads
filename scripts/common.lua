merge_ignore = {
	"__index",
	"parent"
}

mIgnore = function(i)
	if (i==merge_ignore[1] or i==merge_ignore[2]) then
		return true
	else
		return false	
	end
end

function dprintf(s,...)
	io.write("Lua script\t| ")
	io.write(s:format(...))
	print()
end

function _serialize(o,_path)
	if (type(_path) == "nil") then _path="" end
	if type(o) == "number" then
		setData(_path,o)
	elseif type(o) == "boolean" then
		setData(_path,o)
	elseif type(o) == "string" then
	--	setData(_path,string.format("%q", o))
		setData(_path, o)
	elseif type(o) == "function" then
		setData(_path,o)
	elseif type(o) == "table" then
		setData(_path,o)
		for k,v in pairs(o) do
		  	if (mIgnore(k)==false) then
				_serialize(v,_path.."::"..k)
			end	
		end
	else
		error("cannot serialize a " .. type(o))
	end
end

function _serialize2(o,_path)
	if (type(_path) == "nil") then _path="" end
	if type(o) == "number" then
		print(_path,o)
	elseif type(o) == "boolean" then
		print(_path,o)
	elseif type(o) == "string" then
		print(_path,o)
	elseif type(o) == "function" then
		print(_path,o)
	elseif type(o) == "table" then
		for k,v in pairs(o) do
		  	if (mIgnore(k)==false) then
				_serialize2(v,_path.."::"..k)
			end
		end
	else
		error("cannot serialize a " .. type(o))
	end
end


function serializeData(name)
	_serialize(_G[tostring(name)],tostring(name))
end

-- __index = function (table, key)
--	print("get",key)
--	return table[key]
-- end,


_serializable = {
	__newindex = function (table, key, value)
		-- print("set",key,value)
		rawset(table,key,value)
	end
}

_data = {
}

function _data:new(o)
	o = o or {}
	setmetatable(o,_serializable)
	self.__index = self;
	self._self = self;
	return o;
end

function table.merge(t, u)
  local r = {}
  
  process = function(v,w)
  	if type(w) == "table" then
  		v = {}
  		v = table.merge(v,w)
  	else
	    v = w
	end
	return v
  end

  for i, v in pairs(t) do
  	if (mIgnore(i)==false) then
	  	r[i] = process(r[i],v)
	else
		r[i] = v
	end
  end
  for i, v in pairs(u) do
  	if (r[i] == nil) then
	  	if (mIgnore(i)==false) then
		  	r[i] = process(r[i],v)
		else
			r[i] = v
		end
	else
		--print ("'"..tostring(i).."' already exists")
	end
  end
  return r
end

function table.merge2(t, u)
  local r = {}
  
  for i, v in pairs(t) do
		r[i] = v
	end
  for i, v in pairs(u) do
		r[i] = v
  end
  return r
end

data = {
	mouse = {
		x = 0,
		y = 0,
		cx = 0,
		cy = 0,
		rx = 0,
		ry = 0,
		_button = 0,
		button = 0,
		button2 = 0,
		last_button = 0,
		cursors = {},
		click = false,
	},
	keyboard = {},
	keyMod=0,
	keyBind={},
	key_already_processed = false,
	last_key = nil,
	last_key_state = nil,
	counter = 0,
	camera = {
		pos = {
			x = 0,
			y = 0,
			z = 0,
		},
		angle = {
			x = 0,
			y = 0,
			z = 0,
		},
		distance = 100,
		rotate = false,
	},
	models = {},
	outer_grab = false,
	light = true,
	show_gui = true,
}

angle = _data:new({x=0, y=0, z=0})
pos = _data:new({x=0, y=0, z=0})

collisions = _data:new({
	last_result=0,
	test1=0, test2 = 0,test3 = 0 ,test4 = 0,
	intersection = _data:new({
		x=0, y=0, z=0,
		length = function()
			return 0
		end
	})
})


function callObject(object,fn)
	obj = _G[tostring(object)]
	if (obj) then
		f = obj[tostring(fn)]
		if (f) then
			f(obj)
		end
	end
end

function updateData()
end

function open_file(fname)
	buffer = nil
	f = io.open(fname,"r")
	if (f) then
		buffer = f:read("*a")
		assert(f,"Can't open file for reading")
		f:close()
	end
	return buffer
end

-- non-recursive item movement to top of the table
--	-	mostly used in z-ordering to move window to the top of "queue"
--		while thw other windows are moved down to prevent from "nil-gap" making
-- complexity: O(n)
function table.settop(t,v2)
	if (t[1] == v2) then
		return
	end
	local k,v,i,t1,t2
	i=0
	t2 = t[0]
	for k,v in pairs(t) do
		t1 = t[i+1]
		t[i+1] = t2
		if (t1 == v2) then
			t[1] = v2
			t[i+1] = t2
			break
		end
		t2 = t1
		i = i + 1
	end
end
