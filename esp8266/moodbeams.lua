uart.setup(0, 9600, 8, 0, 1, 0)

function writeSerialCommand(command)
	for i, byte in pairs(command) do
		uart.write(0, string.char(byte))
	end
	
	for i = 1, 5 - #command, 1 do
		uart.write(0, string.char(0))
	end
end

function parseCommand(cmd)
	params = {}
	for param in string.gmatch(cmd, "/(%w+)") do
		table.insert(params, param)
	end
	
	for index, param in pairs(params) do
		if index ~= 1 then
			if not checkValue(param) then
				do return false end
			end
		end
	end
	return params
end

function checkValue(value)
	if tonumber(value) then
		value = tonumber(value)
		if value >= 0 and value <= 255 then return true end
	end
	return false
end

server = net.createServer(net.TCP)
server:listen(1503, function(conn)
	conn:on("receive", function(conn, payload)
		success = true
		cmd = string.match(payload, "GET (.+) HTTP/1.1")
		params = parseCommand(cmd);
		
		if params ~= false then
			if params[1] == "setcolor" and #params == 4 then
				writeSerialCommand({ 0xF0, params[2], params[3], params[4] })
			elseif params[1] == "setbrightness" and #params == 2 then
				writeSerialCommand({ 0xF1, params[2] })
			elseif params[1] == "setpreset" and #params == 2 then
				writeSerialCommand({ 0xF2, params[2] })
			elseif params[1] == "definepreset" and #params == 5 then
				writeSerialCommand({ 0xF3, params[2], params[3], params[4], params[5] })
			elseif params[1] == "alternatcolors" and #params == 5 then
				writeSerialCommand({ 0xF4, params[2], params[3], params[4], params[5] })
			elseif params[1] == "twocolor" and #params == 5 then
				writeSerialCommand({ 0xF5, params[2], params[3], params[4], params[5] })
			elseif params[1] == "flashcolor" and #params == 5 then
				writeSerialCommand({ 0xF6, params[2], params[3], params[4], params[5] })
			elseif params[1] == "showrainbow" and #params == 2 then
				writeSerialCommand({ 0xF7, params[2] })
			else
				success = false
			end
		else
			success = false
		end
		
		result = string.format('{ "success":"%s","data":"" }', tostring(success))
		response =	"HTTP/1.1 200 OK\r\n" ..
					"Access-Control-Allow-Origin: *\r\n" ..
					"Content-Length: " .. string.len(result) .. "\r\n" ..
					"Connection: Keep-Alive\r\n" ..
					"Content-Type: text/json\r\n\r\n" ..
					result
		
		conn:send(response)
	end)
	conn:on("sent", function(conn) conn:close() end)
end)