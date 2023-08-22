#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
CYAN="\e[36m"
RESET="\e[0m"

set -ueo pipefail

# Text
echo -e "${GREEN}                _            ${RESET}"
echo -e "${GREEN}               | |           ${RESET}"
echo -e "${GREEN}  _ __ ___   __| | ___   ___ ${RESET}"
echo -e "${GREEN} | '__/ _ \ / _\` |/ _ \ / __|${RESET}"
echo -e "${GREEN} | | | (_) | (_| | (_) | (__ ${RESET}"
echo -e "${GREEN} |_|  \___/ \__,_|\___/ \___|${RESET}"
echo -e "${GREEN}                             ${RESET}"
echo -e "${GREEN}                             ${RESET}"

echo -e "${CYAN}== Setup build ==${RESET}"
meson setup build/
echo -e "${GREEN}== DONE ==${RESET}"

echo -e "${CYAN}== Compilation ==${RESET}"
meson compile -C build/
echo -e "${GREEN}== DONE ==${RESET}"

for var in $@; do
	if [ "$var" == "doc" ]; then
		echo -e "${CYAN}== Creating documentation ==${RESET}"
		doxygen config.ini
		echo -e "${GREEN}== DONE ==${RESET}"
	elif [ "$var" == "run" ]; then
		echo -e "${CYAN}== Running the program ==${RESET}"
		./build/rodoc
		echo -e "${GREEN}== EXIT ==${RESET}"
	elif [ "$var" == "install" ]; then
		echo -e "${CYAN}== Installing the app ==${RESET}"
		meson install -C build/
		echo -e "${GREEN}== DONE ==${RESET}"
	elif [ "$var" == "help" ]; then	
		echo -e "${CYAN}== HELP ==${RESET}"
		echo -e "    doc     -> Create documentation."
		echo -e "    run     -> Run the app."
		echo -e "    install -> Install the app."
	else
		echo -e "${RED}-- Unknown argument '$var'. Try using 'help'. --${RESET}"
	fi
done
