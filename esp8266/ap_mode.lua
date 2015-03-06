function x2c(x) return string.char(tonumber(x, 16)) end
function u(s) return s:gsub("%%(%x%x)", x2c) end

server = net.createServer(net.TCP)
server:listen(80, function(conn)
	conn:on("receive", function(conn, p)
		q = string.match(p, "GET (.+) HTTP/1.1")
		is = false
		if q ~= "/favicon.ico" then
			if q ~= "/" then
				p = {}
				for k, v in string.gmatch(q, "([^?=&]+)=(([^&]*))" ) do p[k] = u(v) end
				file.open("config.lua", "w+")
				file.write("id={"..p['id'].."}:sp={"..p['sp'].."}:u={"..p['u'].."}:p={"..p['p'].."}")
				file.close()
				h = 'Success!'
				is = true
			else
				h = '<!DOCTYPE html><html><body style="font-family: arial"><form><table>'..
					'<tr><td colspan="2"><b>Select Home Wireless</b></td></tr>'..
					'<tr><td>SSID </td><td><input type="text" name="id" required></td></tr>'..
					'<tr><td>SSID Password </td><td><input type="password" name="sp" required></td></tr>'..
					'<tr><td colspan="2"><b>Create Account</b></td></tr>'..
					'<tr><td>Email </td><td><input type="email" name="u" required></td></tr>'..
					'<tr><td>Create Password </td><td><input type="password" name="p" required></td></tr>'..
					'<tr><td>&nbsp;</td><td><input type="submit" value="Save"></td></tr>'..
					'</table></form></body></html>'
			end
			conn:send(h)
			if is then
				tmr.alarm(1, 2500, 0, function() node.restart() end)
			end
		end
	end)
	conn:on("sent", function(conn) conn:close() end)
end)