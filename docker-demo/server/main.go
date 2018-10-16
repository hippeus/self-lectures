package main

import (
	"html/template"
	"log"
	"net/http"
)

var tpl *template.Template

func init() {
	tpl = template.Must(template.ParseFiles("./html/index.html"))
}

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		if err := tpl.ExecuteTemplate(w, "index.html", nil); err != nil {
			log.Fatalf("Could not execute template: %v\n", err)
		}
	})
	err := http.ListenAndServe(":8080", nil)
	log.Fatal(err)
}
