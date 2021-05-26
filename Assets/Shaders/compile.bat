
echo off

goto(){
glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv
read -p "Press enter to continue"
}

goto $@
exit

:(){
rem Windows script here
glslc.exe shader.vert -o vert.spv
glslc.exe shader.frag -o frag.spv
pause
echo %OS%
exit