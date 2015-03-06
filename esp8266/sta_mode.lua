function writeSerialCommand(command)
	for i, byte in pairs(command) do
		uart.write(0, string.char(byte))
	end
	for i = 1, 5 - #command, 1 do
		uart.write(0, string.char(0))
	end
end

uart.setup(0, 9600, 8, 0, 1, 0)
conn=net.createConnection(net.TCP, 0) 
conn:connect(1337, "turningdigital.com")

conn:on("connection", function(conn)
	conn:send(u..":"..p)
end)

conn:on("receive", function(conn, data)
	cmd = {}
	for param in string.gmatch(data, "(%w+)") do
		table.insert(cmd, param)
	end
	
	if cmd[1] ~= nil then
		if cmd[1] == "showcolor" then
			writeSerialCommand({ 0xF0, cmd[2], cmd[3], cmd[4] })
		elseif cmd[1] == "setbrightness" then
			writeSerialCommand({ 0xF1, cmd[2] })
		elseif cmd[1] == "showpreset" then
			writeSerialCommand({ 0xF2, cmd[2] })
		elseif cmd[1] == "definepreset" then
			writeSerialCommand({ 0xF3, cmd[2], cmd[3], cmd[4], cmd[5] })
		elseif cmd[1] == "alternatecolors" then
			writeSerialCommand({ 0xF4, cmd[2], cmd[3] })
		elseif cmd[1] == "twocolor" then
			writeSerialCommand({ 0xF5, cmd[2], cmd[3] })
		elseif cmd[1] == "flashcolor" then
			writeSerialCommand({ 0xF6, cmd[2] })
		elseif cmd[1] == "showrainbow" then
			writeSerialCommand({ 0xF7, cmd[2] })
		end
	end
end)
