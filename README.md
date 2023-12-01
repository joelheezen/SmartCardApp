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
 
# barcode formats

barcodes are returned as an int, so for example, EAN-13 is denoted as 32

https://developers.google.com/android/reference/com/google/mlkit/vision/barcode/common/Barcode#FORMAT_EAN_13

check the link for explanation and complete formats compatible with this scanner.
