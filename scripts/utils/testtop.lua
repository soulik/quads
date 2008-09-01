function Ba(x)
	return {name = (x or "Nothing") }
end

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
		--t[i2+1] = t2
		if (t1 == v2) then
			print ("found at ",i,t[i])
			t[1] = v2
			t[i+1] = t2
			break
		end
		t2 = t1
		i = i + 1
	end
end

A = {
	Ba("one"),
	Ba("two"),
	Ba("three"),
	Ba("four")
}

function dprint()
	local i,k
	for i,k in pairs(A) do
		print (i,k,k.name)
	end
	print("---------------------")
end

table.settop(A,A[3])
dprint()
