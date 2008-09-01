am = {
	__mt = {}
}

function am:new(o)
	obj = o or {}
	obj.__data = {}
	setmetatable(obj,am.__mt)
	return obj
end


function am.__mt.__index(t,k)
	print ("index on "..tostring(t).."["..k.."]")
	return rawget(t.__data,k)
end

function am.__mt.__newindex(t,k,v)
	print ("newindex on "..tostring(t).."["..k.."] = "..tostring(v))
	rawget(t,"__data")[k] = v
end

a = am:new()

a.a = 1
a.a = 2
a.b = am:new()
a.b.c = 1
print (a['a'])
print (a.c)