#!/bin/bash

set -ueo pipefail

echo "<RCC>" > ../resources.qrc
echo "  <qresource prefix=\"/\">" >> ../resources.qrc
for picture in $(ls); do
	printf "    <file>resources/" >> ../resources.qrc
	printf $picture >> ../resources.qrc
	echo "</file>" >> ../resources.qrc
done
echo "  </qresource>" >> ../resources.qrc
echo "</RCC>" >> ../resources.qrc
