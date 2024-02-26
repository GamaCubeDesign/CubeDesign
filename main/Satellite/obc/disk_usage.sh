#!/bin/bash

df -h / | awk 'NR==2 {gsub("%","",$5); print $5}'
