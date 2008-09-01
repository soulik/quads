aaa = {ABC = true, EEE= true}

function isnil(a)
	result = false
	if (type(a) ~= "nil") then
		result = true
	end
	return result
end

result = isnil(aaa["ABC"]) or false

print (aaa["ABC"])
print ())