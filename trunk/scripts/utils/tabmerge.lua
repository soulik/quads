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

a = {
	name = "A",
	description = "EE",
	b = {
		name = "B",
		c = {
			name = "C"
		},
		description = "FF"
	}
}

aa = {
	name = "Aa",
	description = "DD",
	bb = {
		name = "Bb",
		cc = {
			name = "Cc"
		}
	}
}

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
		print ("'"..tostring(i).."' already exists")
	end
  end
  return r
end

aaa = table.merge(a,aa)

_serialize2(a,"a")
print("---------------")
_serialize2(aa,"aa")
print("---------------")
_serialize2(aaa,"aaa")

