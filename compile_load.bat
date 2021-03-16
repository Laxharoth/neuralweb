::c++ -c .\src\function.cpp .\test.cpp -I.\header
::c++ .\test.o  .\function.o -o main.exe
c++ -c  .\src\neuron.cpp .\src\layer.cpp .\src\web.cpp .\src\function.cpp .\src\web_file_handle.cpp .\test_web.cpp -I.\header -DLOAD_FILE=1
c++ neuron.o layer.o web.o function.o web_file_handle.o test_web.o  -o main.exe
del *.o
.\main.exe