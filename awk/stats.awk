#!/usr/bin/gawk -f
BEGIN {
	printf "Processing ."

	FS=","
    
    if(ARGC == 1) {
        exit
    }
}

NR > 1 && NF == 3 {
	printf "."
	info[$1][length(info[$1])+1] = $3 - $2
}

END {
	printf ". done!\n"

	for(command in info) {
		len = length(info[command])
		print command " has been invoked " length(info[command]) " time(s)"

		sum = 0
		max = info[command][1]
		min = info[command][1]
		
		for(i in info[command]) {
			cur = info[command][i]
			sum += cur
			max = cur > max ? cur : max
			min = cur < min ? cur : min
		}

		printf(" max: %d, min: %d, mean time: %.3f\n", max, min, sum / len)
	}
}
