#!/bin/bash
sudo mkdir -p /usr/share/fonts/truetype/myfonts && \
sudo cp ~/*.ttf /usr/share/fonts/truetype/myfonts && \
sudo chown root.root /usr/share/fonts/truetype/myfonts/*.ttf && \
cd /usr/share/fonts/truetype/myfonts && \
sudo mkfontdir && \
cd .. && \
fc-cache

