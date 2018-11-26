#!/usr/bin/env bash

if [[ -t 1 ]]; then
	C_RED="\033[31;01m"
	C_GREEN="\033[32;01m"
	C_YELLOW="\033[33;01m"
	C_BLUE="\033[34;01m"
	C_PINK="\033[35;01m"
	C_CYAN="\033[36;01m"
	C_NO="\033[0m"
fi

################################################################################
###                                FUNCTIONS                                 ###
################################################################################

function clean()
{
	rm mine
	rm nm
}

function is_directory()
{
	cd $1 1>/dev/null 2>/dev/null

	if [ $? != 0 ]; then
		echo "abort: \"${1}\" is not a directory"
		exit
	fi

	cd - 1>/dev/null
}

function is_nm()
{
	ls | grep ft_nm 1>/dev/null 2>/dev/null

	if [ $? == 1 ]; then
		echo "abort: ft_nm binary not found"	
		exit
	fi
}

function run()
{
	for index in $(ls ${1});
	do
		filepath="${1}/${index}"

		nm ${filepath} 1>/dev/null 2>/dev/null
		if [ $? == 0 ]; then
			printf "${filepath}: "
			./ft_nm ${filepath} > mine
			nm ${filepath} > nm
			diff mine nm 1>/dev/null && printf " ${C_GREEN}OK${C_NO}\n" || printf " ${C_RED}ERR${C_NO}\n"
		fi;
	done
}

################################################################################
###                                   MAIN                                   ###
################################################################################

if [ ${1} ]; then
	is_directory ${1}
	is_nm
	run ${1}
	clean
else
	printf "usage: run.sh [directory]\n"
fi
