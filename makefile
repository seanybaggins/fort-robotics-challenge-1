ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

.PHONY: image
image:
	docker build -t ceedling-image $(ROOT_DIR)

.PHONY: test
test: image
	docker run -t -v $(ROOT_DIR):/project ceedling-image ceedling

.PHONY: build
build: image
	docker run -t -v $(ROOT_DIR):/project ceedling-image ceedling release

.PHONY: run
run: build
	sudo $(ROOT_DIR)/build/release/project.out

.PHONY: clean
clean:
	docker run -t -v $(ROOT_DIR):/project ceedling-image ceedling clean


