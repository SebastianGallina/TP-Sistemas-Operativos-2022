clear
echo Eliminando logs...
cd ../consola
rm consola.log -f
cd ../cpu
rm CPU.log -f
cd ../kernel
rm kernel.log -f
cd ../memoria
rm memoria.log -f

echo
echo Compilando shared...
echo
cd ../shared
make clean all

echo
echo Compilando modulos...

echo
echo Compilando consola...
echo
cd ../consola
make clean all

echo
echo Compilando kernel...
echo
cd ../kernel
make clean all

echo
echo Compilando CPU...
echo
cd ../cpu
make clean all

echo
echo Compilando Memoria...
echo
cd ../memoria
make clean all