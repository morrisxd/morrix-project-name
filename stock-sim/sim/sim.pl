#!/usr/bin/perl
# this program is used to handle multiple stock data file
# mount the 2009_TEXT_ALL.ISO into the file system and, run this script 
# without parameters.

$sellout_price = 0.0;

$sell_or_buy = 0;

#
# flag(s)
#
$nothing_happened = 99;
$promotion_flag = 3;
$decrease_flag = 11;
$sell_flag = 1;
$buyin_flag = 2;
$sell_out_flag = 12;



$cnt_sellout = 0;
$cnt_buyin = 0;
 
$trade_date = 1;
$trade_time = 0;
$trade_time_line = 230;
$op_time_line = "14:58";
$op_clean_time_line = "14:59";
$no_trade_time_line = $trade_time_line + 6;

# $count is used to trace the time for every minutes.
$count = 0;
$inc_or_dec = 0; # price is increase or decrease

###################################################
# threshold for selling
$stop_threshold = 0.96;
$buyin_threshold = 0.99; #0.96 is same as 0.96, better than 0.999 when increase
print "stop_threshold(" . $stop_threshold . "), buyin_threshold(" . $buyin_threshold . ")\n";

$increase_base = 0.0;
###################################################

# how many stock we have, if we have stock, then we 
# have NO money
$total_stock = 0;

# how many money we have, if we have moeny, then we
# have NO stock 
$total_money = 100000.00; 	# 100 thousand rmb

# no use for now
$day_close_price = 0.0;

# yesterday/history close price
$y_close_price = 0.0;
$buyin_price = 0.0;


# $trace_price represent that the price we trace for using $stop_threshold only
# when we has stock(s) in hand.
$trace_price = 0.0;


$watch_price = 0.0;

# currently/today's close price
$close_price = 0;

# no use now

# currently, we have money , or stock
$has_stock = 0;

# no use for now
$oneday_without_anything = 0;

$yesterday = 0;
$find_next_day = 0;

##############################################################################
#################### year 2009: bit increase  ################################
##############################################################################
#
# 0.98/$buyin_threshold=0.99 will get profit
#
# @filelist = `find ./sd.dir/20110111_213901/txtall -name SH1A0001.TXT -print`;
## @filelist = `find ./sd.dir/20110111_213901/txtall -name SH601919.TXT -print`;
@filelist = `find ./sd.dir/20110111_213901/txtall -name SH600875.TXT -print | sort -th -n -k2`;
# @filelist = `find ../sd/sd.dir/txtall -name SH1A0001.TXT -print`;

##############################################################################
#################### year 2008: terrible loss  ###############################
##############################################################################
#
# 0.98/$buyin_threshold=0.94 will get profit
#
# @filelist = `find ./sd.dir/20110208_163327/txtall -name SH600875.TXT -print | sort -th -n -k2`;
#
# 0.98/$buyin_threshold=0.93 will get profit
#
# @filelist = `find ./sd.dir2/20110208_163327/txtall -name SH601919.TXT -print | sort -th -n -k2`;
# @filelist = `find ./sd.dir2/txtall -name SH600875.TXT -print | sort -th -n -k2`;
# @filelist = `find ../sd/sd.dir/txtall -name SH600875.TXT -print | sort -th -n -k2`;

print "command line arg is(". $ARGV[0] . ")\n";

foreach $file (@filelist) {
	$find_next_day = 0;
	$oneday_without_anything = 0;
	$sell_out_flag = 0;
	$count = 0;
	chomp($file);
	print "\t\t\t[" . $file . "]\n";
	print "\t\t\t[" . $file . "]\n";
	print "\t\t\t[" . $file . "]\n";
	print "stop_threshold(" . $stop_threshold . "), buyin_threshold(" . $buyin_threshold . ")\n";
	close datafile;
	open datafile, "< $file";

	# each file is a new day	
	print "we found a new day2 in a new file\n";

	foreach $line (<datafile>) {
		($date, $time, $open_price, $top_price, $bottom_price, 
			$close_price, $end1, $end2) = split /\s+/, $line;

		#########################################################
		#########################################################
		# we find the nex day now ...
		#########################################################
		#########################################################
		if (1 == $find_next_day)
		{
			#################################################
			# we skip all the unused minutes if we find the 
			# $find_next_day == 1
			# When we find the next day, 
			# we set the $find_next_day = 0
			#
			if ($yesterday eq $date) {
				printf "same file&day [" .$date
					. "][".$time
					. "],trace_price(".$trace_price
					. "), close_price(" . $close_price 
					. ")\n";
				if ($has_stock == 0) {
					$watch_price = $close_price;
				}

				if ($sell_or_buy == $promotion_flag) {
					$trace_price = $close_price;
					print "set trace_price(" .$close_price.") now, because promotion just now(promotion_flag=" . $promotion_flag. ")\n";
				print "________________________________________________________________________\n";
				}
			
				if ($sell_or_buy == $nothing_happened
					|| $sell_or_buy == $sell_flag) {
					if ($increase_base > $close_price) {
						if ($time ge $op_time_line) {
						print "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}\n";
						print "decrease increase_base to (" . $close_price. ")old($increase_base)\n";
						$increase_base = $close_price;
#						$sellout_price; 
						}
					}
				}

				next;
			}

			#
			# when we find another day in the same file.
			#
			if ($yesterday ne $date) {
				print "we found a new day in the samefile[" 
					. $date . "]\n";
				$count = 0;
				$find_next_day = 0;
				if (0 == $yesterday) {
					printf "This is the first day!\n";
					$yesterday = $date;
				}

				# i think this is NOT necessary
				if ($increase_base > $watch_price
						&& $has_stock == 0 
						&& $sell_or_buy != $sell_flag) {
#					$increase_base = $watch_price;
#					print "we SET increase_base(" 
#						. $increase_base . ")\n";
					;
				}

				# just sold out yesterday 
				# i think this is NOT necessary
				#
				if ($sell_or_buy == $sell_flag) {
#					$increase_base = $watch_price;
#					print "just sold, set increase_base("
#						. $increase_base . ")\n";
					;
				}
				
				if ($sell_or_buy == $buyin_flag) {
					;
				}

				$sell_or_buy = 0;	# clear the flag
			}
		} # end of find next day

################################################################	
################################################################	
#####   normally data processing ###############################	
################################################################	
################################################################	


		$sell_or_buy = 0;
		printf "[" . $date . "-" . $time 
			. "] close_price(" . $close_price . "),trace(" 
			. $trace_price. ")(" . ($trace_price * $stop_threshold)
 			. "),base(".$increase_base
			. ")("; 
printf ("%.3f", $increase_base / $buyin_threshold);
printf ( ")\n");

		#########################################################
		# we buy in at 14:50 whenever the price is OK
		# which means the price increases
		#
		if (0 == $has_stock)
		{
			# we wait until 2:50
#			if ($count < $trade_time_line) {
			if ($time lt $op_time_line) {
				$count ++;
				$yesterday = $date;
				next;
			} 

#			if ($trace_price < $close_price
#			&& 
			if ($increase_base < ($close_price * $buyin_threshold)){
				$y_open_price = $open_price;
				$y_top_price = $top_price;
				$y_bottom_price = $bottom_price;
	
				print "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
				print "<=== [" . $trade_date 
					. "] day, we BUY-IN at cnt(". $count 
					. "), buyin_price(" . $close_price 
					. "), base(" . $increase_base
					. "),  amount(" . $total_stock 
					. ")\n";
				print '$stop_threshold(' . $stop_threshold
					. '),$buyin_threshold('.$buyin_threshold
					. ')$increase_base(' .$increase_base
					. ")\n";

				$buyin_price = $close_price;
				$trace_price = $close_price;
				$total_stock = $total_money / $buyin_price;
				$total_money = 0;
				$has_stock = 1;

				$trade_date ++;
				$oneday_without_anything = 1;
				$yesterday = $date;
				$find_next_day = 1;
				$sell_or_buy = $buyin_flag;
				$cnt_buyin ++;
				# last;
				next;
			}

		}  # end of buy in

		
		#########################################################
		#
		# we sell out the stock when never the price
		# drop down over $stop_threshold
		#
		if ((1 == $has_stock) 
			&& ($trace_price * $stop_threshold)  >= $close_price)
		{
			$total_money = $total_stock * $close_price;
			$has_stock = 0;
			# we mark the $increase_base at once when we sell-out
			$increase_base = $close_price;
			$sellout_price = $close_price;

			print ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
			print "===>\n===> day(" . $trade_date 
				. "), SELL-OUT at(" . $close_price 
				. "), buyin_price(" . $buyin_price
				. "), date(" . $trade_date 
				. "),\n===> loss(" . $close_price / $buyin_price
				. "), Today is end with money(" . $total_money 
				. ")\n";
			print '===> $stop_threshold(' . $stop_threshold
				. '), $buyin_threshold(' . $buyin_threshold
				. ')$increase_base(' .$increase_base
				. ")\n===>\n";

			$total_stock = 0;
			$count ++;
			$trade_date ++;
			$oneday_without_anything = 1;
			$yesterday = $date;
			$find_next_day = 1;
			$sell_or_buy = $sell_flag;
			$cnt_sellout ++;
			# last; 	# we jump to the next day.
			next;
		} # end of sell out


		#########################################################
		#
		# we increase the sell out stop-loss line if the price increase
		#
#		if ($count >= ($trade_time_line + 5)
		if ($time gt $op_time_line
				&& 1 == $has_stock 
				&& $trace_price< $close_price) {
			print "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
			print "[" . $trade_date 
				. "] day, PROMOTION on (" . $close_price 
				. ")count(" . $count 
				. ")old trace_price(" . $trace_price
				. ")\n";
			print '$stop_threshold(' . $stop_threshold
				. '),$buyin_threshold(' . $buyin_threshold
				. ')$increase_base(' .$increase_base
				. ")\n";
			$trace_price = $close_price;
			$count ++;
			$trade_date ++;
			$oneday_without_anything = 1;
			$yesterday = $date;
			$find_next_day = 1;
			$sell_or_buy = $promotion_flag;
			# last;
			next;
		} # end of promotion

		#########################################################
		#
		# if nothing to do which means, 
		# the price does NOT drop under our $stop_threshold, 
		# NOR does it increase.
		#
		$count ++;
#		if ($count >= $no_trade_time_line) {
		if ($time ge $op_clean_time_line) {
			if ($has_stock == 0 
				&& $increase_base > $close_price) {
				print "dec base at close_price($close_price)\n";
				$increase_base = $close_price;
				$sell_or_buy = $decrease_flag;
			}

			$yesterday = $date;
			$find_next_day = 1;
			$sell_or_buy = $nothing_happened;
			next ;
		}
	}
} #end of handling all files




print "###############################################\n";
print "###############################################\n";
print "###############################################\n";
print "now we has money(" . $total_money 
	. "), amount(" . $total_stock 
	. "), a-val(".$total_stock * $close_price 
	. ")\n";
print "stop_threshold(" . $stop_threshold 
	. "), buyin_threshold(". $buyin_threshold
	. ")\n";
print "cnt_sellout(" . $cnt_sellout . "), cnt_buyin(" . $cnt_buyin . ")\n";

foreach $file (@filelist) {
	# print "filelist is: " . $file . "\n";
}

print "filename[$filelist[0]]\n";

print "###############################################\n";
print "###############################################\n";
print "###############################################\n";
