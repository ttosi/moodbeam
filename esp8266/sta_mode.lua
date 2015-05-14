conn = net.createConnection(net.TCP, 0) 
conn:connect(1337, "tdc2.turningdigital.com")

conn:on("connection", function(conn)
	conn:send(u .. ":" .. p)
end)

conn:on("receive", function(conn, data)
	if data ~= nill then
		if data == "ACK" then
			tmr.stop(2)
		else
			for param in string.gmatch(data, "(%w+)") do
				uart.write(0, string.char(param))
			end
			uart.write(0, string.char(10))
		end
	end
end)

-- 	Send heartbeat to server every 5 minutes and wait
--	for an acknowlegdement. If no ACK is received within 15
--	seconds, restart (which reconnects to the server).
tmr.alarm(1, 300000, 1, function()
	conn:send(u .. ":heartbeat")
	tmr.alarm(2, 15000, 1, function()
		node.restart()
	end)
end)