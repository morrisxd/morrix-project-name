dirname=`find . -name "WDS*" -print -maxdepth 1`

for dir in $dirname 
do 
echo $dir
cd $dir
./build_winmon.pl && cd ..
# ls -l build* && cd ..

done

echo All WinMon Version Build successfully !


