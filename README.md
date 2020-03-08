## godot-wow-srp
A C++ Godot module for World of Warcraft client-side SRP   
# How to use
This was tested with Godot 3.2 stable, it might work with older or newer versions.  
  
  
Copy everything into `<godot-source>/modules/wow_srp`, and compile Godot normally.   
Make a new scene, add whichever node, and set this as the code for it:   
```   
func _ready():
	var wowsrp = Wow_SRP.new()
	var username = "TEST"
	var password = "TEST"
	var B = "31396E76E6BC4C2BAF836FC8437162FEFC14DD57107B3537D25015818ABB12D"
	var g = "7"
	var N = "894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7"
	var s = "F3BDF38231BD4FC33D36086CB7F27246511038C0FB6C53260560E58A91EDE97B"
	wowsrp.step1(username, password, B, g, N, s)
	print(wowsrp.get_S())
```   
It should print out `34F76A456B27321F11FBE5BEB26C96B876D00B1357E7EAFCD7D6B556EC3F8A04` if everything is working properly   
