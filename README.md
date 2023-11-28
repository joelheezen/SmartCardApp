# SmartCardApp
App to add barcodes to a smartdevice


# reading the saved data
use sharedPreferences to get the StringSet this gets a list in the following format:

```
val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
val L = sharedPref.getStringSet("barcodes", HashSet()) // L = ("name0/number0/format0", "name1/number1/format1", ...etc)
```

then split using a delimiter like so:

```
L.forEach{
    val str = it
    val delimiter = "/"
    val values = str.split(delimiter)
    values[0] = name
    values[1] = number
    values[2] = format
    }
```
 
