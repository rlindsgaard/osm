ASSIGNMENT=G5
default:
	mkdir $(ASSIGNMENT)
	cp -r report/ $(ASSIGNMENT)
	cp -r source/ $(ASSIGNMENT)
	tar -czf lindsgaard-bjerregaard-uldall-$(ASSIGNMENT).tar.gz $(ASSIGNMENT)

clean:
	rm -rf $(ASSIGNMENT)
	rm lindsgaard-bjerregaard-uldall-$(ASSIGNMENT).tar.gz
