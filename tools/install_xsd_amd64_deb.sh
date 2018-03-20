#!/bin/bash
pushd /tmp
	wget https://www.codesynthesis.com/download/xsd/4.0/linux-gnu/x86_64/xsd_4.0.0-1_amd64.deb
	sudo dpkg -i /tmp/xsd_4.0.0-1_amd64.deb
popd
