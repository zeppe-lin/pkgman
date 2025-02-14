HACKING
=======


Documentation
-------------

* Generate online references to the manual pages in README.md from vim:

```
r:!ls *.[0-9] | sed 's/^man\/\(.*\)/- \[\1\]\(https:\/\/zeppe-lin.github.io\/\1.html\)/g' 
```
