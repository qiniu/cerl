import json, sys, string

dom = json.load(sys.stdin)

defs = dom["sentences"]

def genName(name):
	reserved = ["from", "in", "def", "not", "print"]
	if name in reserved:
		return name + "_"
	return name

def generateDef(indent, d):
	if d.has_key("codedef"):
		data = d["codedef"]
		print "code_%s = %s" % (data["name"], data["value"])
	elif d.has_key("typedef"):
		data = d["typedef"]
		typ = data["type"]
		if typ.has_key("named_type"):
			name = data["name"]
			typename = typ["named_type"]["name"]
			print "%s = %s"% (name, typename)
		elif typ.has_key("coded_type"):
			codename = data["name"]
			print "%s = cerl.Code" % codename
		elif typ.has_key("struct"):
			structname = data["name"]
			print "\n"
			print "class %s(object):" % structname
			print ""
			print "\tvt = vtUserType"
			print ""
			print "\tdef __init__(self):"

			vars = typ["struct"]["vars"]
			members = []
			for var in vars:
				name = genName(var["name"])
				vartype = var["type"]["named_type"]["name"]
				isarray = var["type"].has_key("array")
				if isarray:
					vartype = "Array(%s)" % vartype
				members.append((vartype, name, isarray))

			for vtype, vname, _ in members:
				print "\t\tself._%s = %s.default()" % (vname, vtype)

			for vtype, vname, _ in members:
				print ""
				print "\tdef get_%s(self):" % vname
				print "\t\treturn self._%s" % vname
				print ""
				print "\tdef set_%s(self, v):" % vname
				print "\t\tself._%s = %s.check(v)" % (vname, vtype)

			print ""
			for _, vname, _ in members:
				print "\t%s = property(get_%s, set_%s)" % (vname, vname, vname)

			print ""
			print "\tdef __repr__(self):"
			vnames = map(lambda x : x[1], members)
			vnamekeys = string.join(map(lambda x: "%s: %%s" % x, vnames), ", ")
			vnamevalues = string.join(map(lambda x: "repr(self.%s)" % x, vnames), ", ")
			if len(vnames) == 0:
				print "\t\treturn \"#%s}\""
			elif len(vnames) == 1:
				print "\t\treturn \"#%s{%s}\" %% %s" % (structname, vnamekeys, vnamevalues)
			else:
				print "\t\treturn \"#%s{%s}\" %% (%s)" % (structname, vnamekeys, vnamevalues)

			print ""
			print "\t@staticmethod"
			print "\tdef serialize(writer, v):"
			print "\t\twriteType(writer, %s.vt)" % structname
			for vtype, vname, isarray in members:
				print "\t\t%s.serialize(writer, v._%s)" % (vtype, vname)

			print ""
			print "\t@staticmethod"
			print "\tdef unserialize(reader):"
			print "\t\treadType(reader, %s.vt)" % structname
			print "\t\tresult = %s()" % structname
			for vtype, vname, isarray in members:
				print "\t\tresult.%s = %s.unserialize(reader)" % (vname, vtype)
			print "\t\treturn result"
			print ""

			print "\t@staticmethod"
			print "\tdef check(v):"
			print "\t\tif type(v) == %s:" % structname
			print "\t\t\treturn v"
			print "\t\traise TypeError()"
			print ""

			print "\t@staticmethod"
			print "\tdef default():"
			print "\t\treturn %s.default()" % structname
			print ""
		else:
			print d

def processDef(indent, d):
	if d.has_key("codedef") or d.has_key("typedef") or d.has_key("struct"):
		generateDef(indent, d)
	elif d.has_key("server"):
		server = d["server"]
		cname = server["name"]
		print "class %s:" % cname
		print "\tdef __init__(self, address):"
		print "\t\tself.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)"
		print "\t\tself.sock.connect(address)"
		print "\t\tself.buffered = BufferedSocket(self.sock)"
		print ""

		for sentence in server["sentences"]:
			if sentence.has_key("ctor"):
				continue
			assert sentence.has_key("function")
			func = sentence["function"]
			args = func.get("args")
			async= func.get("async") or "sync"
			typ =  func.get("type")
			name = func["name"]
			id   = func["id"]

			if args == None:
				args = []
			args = map(lambda arg: (arg["type"].get("named_type")["name"], arg["type"].has_key("array"), genName(arg["name"])), args)

			argList = string.join(["self"] + map(lambda x: x[2], args), ", ")
			print "\tdef %s(%s):" % (name, argList)

			print "\t\twriter = self.buffered"
			print "\t\theader = MailHeader()"
			print "\t\theader.fid = %s%s" % (id, (async == "async" and " | CERL_FID_ASYNC_MASK" or ""))
			print "\t\tMailHeader.serialize(writer, header)"
			print ""

			for argtype, isarray, argname in args:
				if isarray:
					print "\t\tArray(%s).serialize(writer, %s)" % (argtype, argname)
				else:
					print "\t\t%s.serialize(writer, %s)" % (argtype, argname)
			print "\t\twriter.write('\\0\\0') # vtNull"
			print "\t\twriter.flush()"

			print ""

			if async == "sync" and typ != None:
				print "\t\treader = self.buffered"
				print "\t\tresHeader = MailHeader.unserialize(reader)"
				print ""
				print "\t\tresult = {}"
				print "\t\tcode = cerl.Code.unserialize(reader)"
				print "\t\tresult[\"code\"] = code"

				condCount = 0
				items = typ["coded_type"]["items"]
				for item in items:
					vars = item.get("vars") or []
					if len(vars) > 0:
						if condCount == 0:
							print "\t\tif code == code_%s:" % item["code"]
						else:
							print "\t\telif code == code_%s:" % item["code"]
						condCount += 1

						for var in vars:
							vartype = var["type"]["named_type"]["name"]
							isarray = False
							if var["type"].has_key("array"):
								vartype = "Array(%s)" % vartype
								isarray = True
							varname = var["name"]

							print "\t\t\tvalue = %s.unserialize(reader)" % vartype
							print "\t\t\tresult[\"%s\"] = value" % varname
				print "\t\ttag = reader.read(2)"
				print "\t\tassert tag == '\\0\\0' # vtNull"
				print "\t\treturn result"
				print "\n"
	elif len(d.keys()) == 0:
		return None
	else:
		print d, d.keys(), "\n\n"


print """
import struct, string, socket

vtInvalid		= -1
vtNull			= 0x0000
vtI1			= 0x1000 # char, bool, INT8, UINT8
vtI2			= 0x2000 # INT16, UINT16
vtI4			= 0x3000 # INT32, UINT32
vtI8			= 0x4000 # INT64, UINT64, NInformation
vtR8			= 0x5000 # double
vtR4			= 0x6000 # float
vtUserType		= 0x7000
vtReserved0		= 0x8000
vtReserved1		= 0x9000
vtReserved2		= 0xA000
vtReserved3		= 0xB000
vtReserved4		= 0xC000
vtReserved5		= 0xD000
vtReserved6		= 0xE000
vtReserved7		= 0xF000
vtArrayMask		= 0x0800

ioTypeMask	= 0xf800
ioSizeMask	= 0x07ff


def readType(reader, vt):
	vtType = struct.unpack("H", reader.read(2))[0]
	assert vtType == vt

def writeType(writer, vt):
	writer.write(struct.pack("H", vt))

class Number(object):
	def __init__(self, types, vtType, packFmt, min, max, bytes, default=None):
		self.types = types
		self.vt = vtType
		self.fmt = packFmt
		self.min = min
		self.max = max
		self.bytes = bytes
		self.defaultValue = default

	def check(self, v):
		typ = type(v)
		for t in self.types:
			if typ == t:
				break
		else:
			raise TypeError()

		if  self.min != None and v < self.min or \
			self.max != None and v > self.max:
			raise AttributeError("value not in range")

		
		return v

	def default(self):
		if self.defaultValue != None:
			return self.defaultValue
		return 0

	def write(self, writer, v):
		writer.write(struct.pack(self.fmt, v))

	def read(self, reader):
		value = struct.unpack(self.fmt, reader.read(self.bytes))[0]
		return value

	def serialize(self, writer, v):
		writeType(writer, self.vt)
		self.write(writer, v)

	def unserialize(self, reader):
		readType(reader, self.vt)
		return self.read(reader)



UInt8  = Number([type(0), type(0L)], vtI1, "B", 0, 0xff, 1)
UInt16 = Number([type(0), type(0L)], vtI2, "H", 0, 0xffff, 2)
UInt32 = Number([type(0), type(0L)], vtI4, "I", 0, 0xffffffff, 4)
UInt64 = Number([type(0), type(0L)], vtI8, "Q", 0, 0xffffffffffffffff, 8)

Int8  = Number([type(0), type(0L)], vtI1, "b", -0x80, 0x7f, 1)
Int16 = Number([type(0), type(0L)], vtI2, "h", -0x8000, 0x7fff, 2)
Int32 = Number([type(0), type(0L)], vtI4, "i", -0x80000000, 0x7fffffff, 4)
Int64 = Number([type(0), type(0L)], vtI8, "q", -0x8000000000000000, 0x7fffffffffffffff, 8)

Byte = UInt8

class Char:
	vt = vtI1

	@staticmethod
	def check(v):
		if type(v) == type(""):
			return v
		raise TypeError()

	@staticmethod
	def default():
		return "\\0"

	@staticmethod
	def serialize(writer, v):
		writeType(writer, Char.vt)
		Char.write(writer, v)

	@staticmethod
	def unserialize(reader):
		readType(reader, Char.vt)
		return Char.read(reader)

	@staticmethod
	def write(writer, v):
		writer.write(v[0])

	@staticmethod
	def read(reader):
		return reader.read(1)


class Bool:
	vt = vtI1

	@staticmethod
	def check(v):
		if type(v) == type(True):
			return v
		raise TypeError()

	@staticmethod
	def default():
		return False

	@staticmethod
	def serialize(writer, v):
		writeType(writer, Bool.vt)
		Bool.write(writer, v)

	@staticmethod
	def unserialize(reader):
		readType(reader, Bool.vt)
		return Bool.read(reader)

	@staticmethod
	def write(writer, v):
		UInt8.serialize(writer, v and 1 or 0)

	@staticmethod
	def read(reader):
		v = UInt8.unserialize(reader)
		v = v and True or False
		return v

class NInformation(object):
	vt = vtI8

	def __init__(self, ip="", port=0):
		assert type(ip) == type("")
		assert type(port) == type(0)
		self.ip = ip
		self.port = port

	def toInteger(self):
		ip = socket.inet_aton(self.ip)
		ip = struct.unpack(">I", ip)[0]
		return ip | (self.port << 32)

	def __repr__(self):
		return "#NInformation{ip: %s, port: %s}" % (self.ip, self.port)

	@staticmethod
	def check(v):
		if type(v) == type((1,1)) and type(v[0]) == type("") and type(v[1]) == type(1):
			return NInformation(v[0], v[1])
		elif type(v) == NInformation:
			return v
		raise TypeError()

	@staticmethod
	def default():
		return ("", 0)

	@staticmethod
	def write(writer, v):
		writer.write(struct.pack("Q", v.toInteger()))

	@staticmethod
	def read(reader):
		result = NInformation()
		ipdata = string.join(reversed(reader.read(4)), "")
		result.ip = socket.inet_ntoa(ipdata)
		result.port = struct.unpack("I", reader.read(4))[0]
		return result

	@staticmethod
	def serialize(writer, v):
		writeType(writer, NInformation.vt)
		NInformation.write(writer, v)

	@staticmethod
	def unserialize(reader):
		readType(reader, NInformation.vt)
		return NInformation.read(reader)

class Array:
	packTypes = [UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Char, Bool, Byte, NInformation]

	def __init__(self, typ):
		self.typ = typ
		if self.typ in Array.packTypes:
			self.vt = vtArrayMask | self.typ.vt
		else:
			self.vt = vtArrayMask | vtUserType

	def check(self, v):
		for i in v:
			self.typ.check(i)
		return v

	def default(self):
		if self.typ == Char:
			return ""
		return []

	def serialize(self, writer, v):
		if ioSizeMask > len(v):
			writer.write(struct.pack("H", self.vt | len(v)))
		else:
			writer.write(struct.pack("H", self.vt | ioSizeMask))
			UInt32.write(writer, len(v))

		if self.typ == Char:
			writer.write(v)
		else:
			if self.typ in Array.packTypes:
				for i in v:
					self.typ.write(writer, i)
			else:
				for i in v:
					self.typ.serialize(writer, i)

	def unserialize(self, reader):
		tag = struct.unpack("H", reader.read(2))[0]
		assert tag & self.vt

		if tag == (self.vt | ioSizeMask):
			count = UInt32.read(reader)
		else:
			count = ioSizeMask & tag

		if self.typ == Char:
			result = reader.read(count)
		else:
			result = []
			if self.typ in Array.packTypes:
				for i in range(count):
					item = self.typ.read(reader)
					result.append(item)
			else:
				for i in range(count):
					item = self.typ.unserialize(reader)
					result.append(item)
	
		return result

String = Array(Char)

class TmpModule: pass
cerl = TmpModule()
setattr(cerl, "code_ok", 0x0002)
setattr(cerl, "code_false", 0x0000)
setattr(cerl, "code_true", 0x0001)
setattr(cerl, "code_error", 0xffff)
setattr(cerl, "code_unknown_error", 0xfff6)
setattr(cerl, "code_exists_already", 0xfff7)
setattr(cerl, "code_undefined", 0xfff8)
setattr(cerl, "code_socket_error", 0xfff9)
setattr(cerl, "code_encounter_exception", 0xfffa)
setattr(cerl, "code_input_error", 0xfffb)
setattr(cerl, "code_unknown_fid", 0xfffc)
setattr(cerl, "code_format_error", 0xfffd)
setattr(cerl, "code_timeout_error", 0xfffe)

setattr(cerl, "Code", UInt32)

setattr(cerl, "UInt32", UInt32)


CERL_FID_ASYNC_MASK = 0x80000000


class MailHeader:
	counter = 0

	def __init__(self):
		MailHeader.counter += 1

		self.cbHeader = 16
		self.reserved = 0
		self.magic = 0x99008633
		self.mailno = MailHeader.counter
		self.fid = 0

	@staticmethod
	def serialize(writer, v):
		writer.write(struct.pack("HHIII", v.cbHeader, v.reserved, v.magic,
				v.mailno, v.fid))

	@staticmethod
	def unserialize(reader):
		headerData = reader.read(16)
		h = MailHeader()
		h.cbHeader, h.reserved, h.magic, h.mailno, h.fid = \
				struct.unpack("HHIII", headerData)
		return h

class BufferedSocket:
	BUFSIZE = 4096

	def __init__(self, sockobj):
		self.sock = sockobj
		self.readBuffer = ""
		self.writeBuffer = ""
	
	def read(self, bytes):
		readlen = min(len(self.readBuffer), bytes)
		result = self.readBuffer[:readlen]
		self.readBuffer = self.readBuffer[readlen:]
		if readlen == bytes:
			return result

		bytes -= readlen
		while bytes > 0:
			recvlen = max(BufferedSocket.BUFSIZE, bytes)
			data = self.sock.recv(recvlen)
			if len(data) >= bytes:
				result += data[:bytes]
				self.readBuffer = data[bytes:]
				return result
			if len(data) == 0:
				if len(result) > 0:
					return data
				return False
			result += data
			bytes -= len(data)
		return result

	def write(self, data):
		self.writeBuffer += data

	def flush(self):
		while len(self.writeBuffer) > 0:
			bytes = self.sock.send(self.writeBuffer)
			if bytes <= 0:
				return False
			self.writeBuffer = self.writeBuffer[bytes:]
		return True

####################### sdl generation ##########################
"""

print "code_ok = cerl.code_ok"
print "code_error = cerl.code_error"
print "code_true = cerl.code_true"
print "code_false = cerl.code_false"
print "\n"

for d in defs:
	processDef("", d)

