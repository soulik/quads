--	GL_GL_VERSION_1_1										 = 1 ,

for line in io.lines() do
	str = string.gsub(line,"%s*([^%s]+)%s*=%s*([^%s]+)%s*,","\t%1\t= %2,")
	print(str)
end
