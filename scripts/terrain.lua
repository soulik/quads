require('scripts/common')
require('scripts/gl')
require('scripts/map')

terrain = {
}

function terrain:prepare()
	gl.lists[0] = gl.GenLists(1)
	gl.NewList(gl.lists[0],gl_enum.GL_COMPILE);

		gl.Enable(gl_enum.GL_TEXTURE_2D);
		gl.BindTexture(gl_enum.GL_TEXTURE_2D,tex['animated01'].id)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA);
		gl.Enable(gl_enum.GL_BLEND);
		gl.Begin(gl_enum.GL_QUADS);
		gfield = {};
	
		for y=0, map.height-1, 1 do
	
			for x=0, map.width-1, 1 do
				gfield[0] = map.data[x][y].z
				gfield[1] = map.data[x+1][y].z
				gfield[2] = map.data[x][y+1].z
				gfield[3] = map.data[x+1][y+1].z
	
				gl.Color3f(0.5*(1-gfield[0]/50)+0.2,	0.5*(gfield[0]/50),	0);
				gl.Normal3f(0,0,-1)
	
				gl.TexCoord2f(0,0);
				gl.Vertex3f(x*8,	y*8,	gfield[0]);
	
				gl.TexCoord2f(-1,0);
				gl.Vertex3f(x*8+8,	y*8,	gfield[1]);
	
				gl.TexCoord2f(-1,-1);
				gl.Vertex3f(x*8+8,	y*8+8,	gfield[3]);
	
				gl.TexCoord2f(0,-1);
				gl.Vertex3f(x*8,	y*8+8,	gfield[2]);
			end
		end
		gl.End();

		gl.Disable(gl_enum.GL_BLEND);
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE);
	gl.EndList()
end

function terrain:draw()
	if (gl.lists[0] and gl.lists[0]>0) then
		gl.CallList(gl.lists[0])
	end

end