ORG = me-box
REPO = mirage-net-psock

DOCKER=docker run -ti -v $$(pwd -P):/mirage-net-psock $(ORG)/$(REPO)

.PHONY: build-docker
build-docker:
	docker build -t $(ORG)/$(REPO) .

.PHONY: build-osx
build-osx:
	$(DOCKER) dune build @all

.PHONY: clean-osx
clean-osx:
	$(DOCKER) dune clean

.PHONY: build
build:
	dune build @all

.PHONY: clean
clean:
	dune clean

.PHONY: test
test:
	dune runtest

.PHONY: install
install:
	dune build @install
	dune install

.PHONY: uninstall
uninstall:
	dune uninstall

.PHONY: distrib
distrib:
	dune-release tag
	dune-release distrib

.PHONY: public
publish:
	dune-release publish
	dune-release opam pkg
	dune-release opam submit

.PHONY: release
release: distrib publish
