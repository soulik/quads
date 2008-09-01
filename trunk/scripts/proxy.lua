require ('scripts/common')

LuaData = nil

qLuaMem = {}

function qLuaMem:Init()
	LuaData = mem.new(1)
	dprintf("LuaData Root set")
end

function getDataInfo(_table)
	return tostring(_table).." MT: "..tostring(getmetatable(_table))
end

function qLuaMem:Benchmark()
	local d1 = mem.new()
	local ddd = d1
	local num = 1000
	local time1
	local time2
	local temp
	dprintf("Small qLuaMem benchmark: ")

	time1 = qE.getTickCount()	
	for i = 1,num do
		ddd.data = mem.new()
		ddd = ddd.data
	end
	time2 = qE.getTickCount()	- time1

	dprintf("It took "..tostring(time2).." ms to create recursively "..tostring(num).." qLuaMem tables")
	
	ddd = d1

	time1 = qE.getTickCount()	
	for i = 1,num do
		ddd[i] = i
	end
	time2 = qE.getTickCount()	- time1

	dprintf("It took "..tostring(time2).." ms to fill "..tostring(num).." items")

	time1 = qE.getTickCount()	
	for i = 1,num do
		ddd[i] = i
	end
	time2 = qE.getTickCount()	- time1

	dprintf("It took "..tostring(time2).." ms to fill "..tostring(num).." items again")

	time1 = qE.getTickCount()	
	for i = 1,num do
		temp = ddd[i]
	end
	time2 = qE.getTickCount()	- time1

	dprintf("It took "..tostring(time2).." ms to get "..tostring(num).." items")
end

function testProxy()
	print("Total memory used before benchmark by Lua: "..string.format("%0.2f kB",(collectgarbage("count"))))
	qLuaMem.Benchmark()
	print("Total memory used after benchmark by Lua: "..string.format("%0.2f kB",(collectgarbage("count"))))
	collectgarbage("collect")
	print("Total memory used after benchmark & GC by Lua: "..string.format("%0.2f kB",(collectgarbage("count"))))
end
