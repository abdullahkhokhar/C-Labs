#!/bin/bash
declare -i assign_score=0
declare -i assign_total=0
declare -i tut_score=0
declare -i tut_total=0

declare -i tuts=0

score_of_feedback () {
	#Bellow I use tail with the option -n to get only the last line of the file
	grade=`tail -n 1 $1 | tr -d [:blank:]`
	#now we have the last line of the file trimmed down stored in grade

	IFS='/' read -ra grArray <<< "$grade"
	score=${grArray[0]}
	max=${grArray[1]}

	#increment the numerator
	let "$3 += $score"
	#increminent the denominator
	let "$4 += $max"
	echo "$2: $score / $max"
}


check_and_sum_tuts () {
	#will go through all the directors that start with t
	for d in t01 t02 t03 t04 t05 t06 t07 t08 t09 t10 t11; do
		if [ -d $d ]
		then
			#now we know that this directory exists
			if [ -f "$d/feedback.txt" ]
			then
				score_of_feedback $d/feedback.txt $d tut_score tut_total
			else
				echo "$d: - / -"
			fi
		else
			echo "$d: - / -"
		fi
	done
}

check_and_sum_assigns () {
	#will go through all the directors that start with t
	for d in a1 a2 a3 a4; do
		if [ -d $d ]
		then
			#now we know that this directory exists
			if [ -f "$d/feedback.txt" ]
			then
				score_of_feedback $d/feedback.txt $d assign_score assign_total
			else
				echo "$d: - / -"
			fi
		else
			echo "$d: - / -"
		fi
	done
}

check_and_sum_assigns
echo ""
check_and_sum_tuts
echo "Assignment Total: $assign_score / $assign_total"
echo "Tutorial Total: $tut_score / $tut_total"
