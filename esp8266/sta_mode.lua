function writeSerialCommand(command)
	for i, byte in pairs(command) do
		uart.write(0, string.char(byte))
	end
	uart.write(0, string.char(10))
end

uart.setup(0, 9600, 8, 0, 1, 0)
conn=net.createConnection(net.TCP, 0) 
conn:connect(1337, "tdc2.turningdigital.com")

conn:on("connection", function(conn)
	conn:send(u..":"..p)
end)

tmr.alarm(1, 300000, 1, function()
	conn:send(u..":heartbeat")
	tmr.alarm(2, 15000, 1, function()
		node.restart()
	end)
end)

conn:on("receive", function(conn, data)
	params = {}
	for param in string.gmatch(data, "(%w+)") do
		table.insert(params, param)
	end
	
	if params[1] ~= nill then
		if params[1] == "ACK" then
			tmr.stop(2)
		else 
			if params[1] == "showcolor" then
				writeSerialCommand({ 0xF0, params[2], params[3], params[4] })
			elseif params[1] == "setbrightness" then
				writeSerialCommand({ 0xF1, params[2] })
			elseif params[1] == "alternatecolors" then
				writeSerialCommand({ 0xF2, params[2], params[3], params[4], params[5], params[6], params[7], params[8] })
			elseif params[1] == "twocolor" then
				writeSerialCommand({ 0xF3, params[2], params[3], params[4], params[5], params[6], params[7] })
			elseif params[1] == "flashcolor" then
				writeSerialCommand({ 0xF4, params[2] })
			elseif params[1] == "showrainbow" then
				writeSerialCommand({ 0xF5, params[2] })
			elseif params[1] == "fade" then
				writeSerialCommand({ 0xF6, params[2] })
			end
		end
	end
end)
