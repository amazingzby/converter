import struct
fileManage=open("model.bin","wb")
magicCode = 0x6d76636d
version   = 0x00050004
magicCode=struct.pack('>I',magicCode)
version=struct.pack('>I',version)
fileManage.write(magicCode)
fileManage.write(version)
fileManage.close()