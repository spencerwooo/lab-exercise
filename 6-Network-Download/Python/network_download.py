import urllib.request
import os

#url = 'http://www.blog.pythonlibrary.org/wp-content/uploads/2012/06/wxDbViewer.zip'
#savepath = '/home/brandon/'

url = input("Please input what would you want?")
savepath = input("where?")

filename = os.path.basename(url)
print ("YOU WILL DOWNLOAD")
urllib.request.urlretrieve(url, os.path.join(savepath, filename))
print("OK,finished!")