a) check what will effect look like if Blending was disabled
a) check what was the starting point (if detph testing worked, maybe go with it)
b) check if the result can be obtained using fragment shader (mayber `mix` will
work?) -> check https://thebookofshaders.com/06/

c) add lightbuffer (texture fully transparent, where light is present and
totally not transparent where light is absent)
