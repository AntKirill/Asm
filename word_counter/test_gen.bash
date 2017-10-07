echo -ne "" | cat > tmp.txt
for ((i=0; i<200; i++))
do
	echo -ne "x" | cat >> tmp.txt
done
echo "" | cat >> tmp.txt