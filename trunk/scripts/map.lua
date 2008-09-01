map = {
	width = 50,
	height = 50,
	data = {}
}

function map:init()
	for x = 0, map.width, 1 do
		map.data[x] = {}
		for y = 0, map.height, 1 do
			map.data[x][y] = {}
		end
	end
end

function genMap(width, height,maxHeight, x, y, t)
	return ((math.sin(x/width*M_PI*2+t) *math.cos(y/height*M_PI*2+t)+1) *maxHeight)
end

function initMap()
--	qE.loadMap2('map1.map')
	map:init()
	maxHeight = 50
	t = 0.0
	M_PI = 3.1416
	qE.clearMap(map.width, map.height)
	for y = 0, map.height, 1 do
		for x = 0, map.width, 1 do
			z = genMap(map.width, map.height, maxHeight, x, y, t)-(maxHeight/2.0)
			map.data[x][y].z = z
			qE.setMap(x,y,z)
			maxHeight = maxHeight * 0.9999
		end
		t = t + 0.1
		maxHeight = maxHeight * 0.98
	end
--	qE.saveMap2('map2.map')
end
