import glob, os

root = 'C:/Users/Vo Thanh Phuong/Desktop/GL/GameJOMK/Resources'

ds = []

def getAllFile(dir, pq):
	for o in os.listdir(dir):
		abc = str(o)
		if abc.find('.png') > 0:
			ds.append(pq + '/' + abc)
		if abc.find('.') == -1:
			getAllFile(dir + '/' + abc, pq + '/' + abc)

getAllFile(root, '.')

print ds