::c++ -c .\src\function.cpp .\test.cpp -I.\header
::c++ .\test.o  .\function.o -o main.exe
::DEBUGGER
::g++  .\src\neuron.cpp .\src\layer.cpp .\src\web.cpp .\src\function.cpp .\src\web_file_handle.cpp .\src\backpropagation.cpp .\test_web.cpp -I.\header
c++ -c  .\src\neuron.cpp .\src\layer.cpp .\src\web.cpp .\src\function.cpp .\src\web_file_handle.cpp .\src\backpropagation.cpp .\test_web.cpp -I.\header
c++ neuron.o layer.o web.o function.o web_file_handle.o backpropagation.o test_web.o -o main.exe
del *.o
::.\main.exe