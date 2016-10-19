#!/usr/bin/env python2
from __future__ import print_function
import spatial_index as si
import csv


def gen_values(dict_data):
    for elem in dict_data:
        lon = float(elem['Longitude'])
        lat = float(elem['Latitude'])
        yield si.Value(si.GeoPoint(lon, lat), elem)


def main():
    with open('./worldcitiespop.txt', 'r') as csv_cities_data:
        cities_data_reader = csv.DictReader(csv_cities_data)
        rtree = si.RTree(gen_values(cities_data_reader))
        print(rtree.nearest(si.GeoPoint(60.6057, 56.8389), 10))



if __name__ == '__main__':
    exit(main())
