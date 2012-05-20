#!/usr/bin/awk -f
# awk -f r.awk 601628.TXT 0.98
#


BEGIN { 
	# we must read the argv firstly, then set it to 0
	searchpattern=ARGV[2]
	print "head of the awk, [1]=" ARGV[1] ", [2]=" ARGV[2] ", [3]=" ARGV[3] ", [0]="ARGV[0]
	numbig = 0
	num = 0
	gate = ARGV[2]
	ARGV[2] = ""
	p1 = 0
	p2 = 0
	p3 = 0
	p4 = 0
	p5 = 0
	p6 = 0
	p7 = 0
	p8 = 0
	p9 = 0
	p10 = 0
	total = 0.0
	cnt = 0
	close1=0
	lowest=0
}

# we do this for each line
{
	print "("   $2   ") \t+ ("    $5   "\t)=	"  $2+$5
	close1=$5
	lowest=$4
	if (gate < close1 - lowest)
		num = num + 1
	else 
		numbig = numbig + 1


	total = total + $5
	cnt = cnt + 1
} 


END {
	print "There were [" num "]:[" numbig "] lines of input, gate=[" gate "], per=[%"  (num/cnt)*100 "]"
	print "average price is:[" total / cnt "], percent=[%" gate / (total/cnt)*100  "]" 
}


