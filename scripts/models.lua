require('scripts/common')
require('scripts/gl')
require('scripts/gl_common')
require('scripts/proxy')

models = {}

function loadModel(fname,t,style)
	qE.loadModel(config.models_dir..tostring(fname),t,style)
	models[fname] = {
		pos = {x=0,y=0,z=0,},
		angle = {x=0,y=0,z=0,}
	}
end

function getModel(fname,t,style)
	qE.loadModel(config.models_dir..tostring(fname),t,style)
	return models[fname]
end

function drawModel(fname,mode)
	local mdl = models[fname]
	gl.PushMatrix()
	gl.Translatef(mdl.pos.x,mdl.pos.y,mdl.pos.z)
	gl.Rotatef(mdl.angle.x,1,0,0)
	gl.Rotatef(mdl.angle.y,0,1,0)
	gl.Rotatef(mdl.angle.z,0,0,1)
	qE.drawModel(config.models_dir..tostring(fname),mode)
	gl.PopMatrix()		
end

function updateModel(fname,mode)
	qE.updateModel(config.models_dir..tostring(fname))
end

function updateModelMaterial(mod_name,mat_name)
	qE.updateModelMaterial(config.models_dir..mod_name,mat_name)
end

function setModelAnimation(fname,oname,aname)
	qE.setModelAnimation(config.models_dir..tostring(fname),tostring(oname),tostring(aname))
end

function models:prepare()
end

transx = 0

function models:draw()
end
