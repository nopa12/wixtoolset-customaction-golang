package main

import "C"
import (
	"time"
)

//export cgoCurrentMillis
func cgoCurrentMillis() C.long {
	return C.long(time.Now().Unix())
}

func main() {

}
