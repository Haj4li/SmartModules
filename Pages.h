#ifndef PAGES_H
#define PAGES_H

const char p_login[] PROGMEM = {
  "<html><head><style>body{display:flex;justify-content:center;align-items:center;height:100vh;background-color:#f1f1f1}form{display:flex;flex-direction:column;align-items:center}input{margin-bottom:10px;padding:8px;width:200px}button{background-color:#007bff;color:#fff;border:none;padding:10px 20px;cursor:pointer}</style></head><body><form action=\"/loginvalid\" method=\"POST\"><input type=\"text\" placeholder=\"Username\" name=\"username\" required><input type=\"password\" placeholder=\"Password\" name=\"password\" required><button type=\"submit\" style=\"background-color:#007bff;color:#fff\">Login</button></form></body></html>"
};

#endif