# Skot 
personal project programming lang   
there is ifs , whiles, fors and memory looks to me like skot is turing complete.

## Memory
-   its not finished, no memory management at all :)
-   it dosnt do anything about memory once it takes it there is no coming back its not a bug its a feature.    
## How to use

write ur code in main.skt and 
```
    ./run.sh
```
example of code:   
```
let a = 1;
for(0..3) {
    if(a == 1) {
        output(a);
    } else if (a == 2) {
        output(a);
    } 
    else {
        output(a);
    }         
    a = a + 1;
}
while(1 == 1) {
    output("turring complete " + ":O");
}
```
