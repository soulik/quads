require('scripts/common');

function d_table(tbl,name)
	buffer = ""
	buffer = buffer..string.format("%s.%s",name,tbl.name)
	buffer = buffer..string.format("%s.%s",name,tbl.name)
	return buffer
end

function inspect(o,o2,o3)
	print("--------------")
	print(o,o2,o3)
	print("--------------")
end
