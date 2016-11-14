#!/bin/bash

locally="false"

if [[ $# -eq 1 &&  ($1 = "-l" || $1 = "--locally") ]]
then
	locally="true"
fi

libraries_directory="$PWD/libraries/"

cmake_arguments="-DCMAKE_BUILD_TYPE=Debug"
sudo="sudo"
if [[ $locally = "true" ]]
then
	cmake_arguments="$cmake_arguments -DCMAKE_INSTALL_PREFIX='$libraries_directory'"
	sudo=""
fi

function install_github()
{
	username=$1
	repo_name=$2
	additional_cmake_arguments=$3
	base_path=`pwd`

	echo "Downloading $username/$repo_name"

	# Download and extract the source
	wget https://github.com/$username/$repo_name/archive/master.zip
	unzip -qq master.zip
	rm master.zip # Must be removed for the next library


	echo "Installing $username/$repo_name"
	# Box2D have different directories structure than usual libraries on GitHub
	if [[ $repo_name = "Box2D" ]]
	then
		cd $repo_name-master/Box2D/Build
	else
		mkdir $repo_name-master/build
		cd $repo_name-master/build
	fi
	cmake $cmake_arguments $additional_cmake_arguments  ..
	make --ignore-errors --quiet --jobs=4
	$sudo make install --ignore-errors --quiet --jobs=4
	cd $base_path # Don't forget to restart from the base path for the next library
}

mkdir "$libraries_directory"
cd "$libraries_directory"
install_github SFML SFML
install_github texus TGUI