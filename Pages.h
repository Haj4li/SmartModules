#ifndef PAGES_H
#define PAGES_H

const char p_login[] PROGMEM = {
  "<html><head><style>body{display:flex;justify-content:center;align-items:center;height:100vh;background-color:#f1f1f1}form{display:flex;flex-direction:column;align-items:center}input{margin-bottom:10px;padding:8px;width:200px}button{background-color:#007bff;color:#fff;border:none;padding:10px 20px;cursor:pointer}</style></head><body><form onsubmit=\"login(event)\"><input type=\"text\" placeholder=\"Username\" id=\"username\" required><input type=\"password\" placeholder=\"Password\" id=\"password\" required><button type=\"submit\" style=\"background-color:#007bff;color:#fff\">Login</button></form><script>function login(e){e.preventDefault();var username=document.getElementById('username').value;var password=document.getElementById('password').value;var xhr=new XMLHttpRequest();xhr.open('POST','/loginvalid',true);xhr.setRequestHeader('Content-type','application/x-www-form-urlencoded');xhr.onreadystatechange=function(){if(xhr.readyState===4&&xhr.status===200){alert('Login successful!');window.location.href='/hello';}else if(xhr.readyState===4&&xhr.status!==200){alert('Invalid username or password!');}};xhr.send('username='+encodeURIComponent(username)+'&password='+encodeURIComponent(password));}</script></body></html>"
};


#endif