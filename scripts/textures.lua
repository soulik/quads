require('scripts/textures/main')
require('scripts/gl')

function updateTextures()
	--tex1()
	--tex2()
end

function loadTex(name,_type)
	local t=0
	if (tex[name]) then
		t = tex[name]
	else
		qE.loadTexture(name,_type)
		t = addTex(name)
	end
	return t.id
end

function addTex(name)
	tex[name] = gl.GetImage(name)
	return tex[name]
end
