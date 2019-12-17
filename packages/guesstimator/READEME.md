# Guesstimator

Guessimator is a library to prepare and run samples.

``unparsedInput -> parsedInput -> sample()``

The Guesstimate app sends ``unparsedInput`` to the Guesstimator library.  This 
first parses it (using the formatters), then samples it (using the samplers)

# Development

All development process is aimed to using tests. This is why only commands
you need are:

to install libraries
```bash
yarn install
```

to run tests
```bash
yarn test
```

Do not forget about ESLinter:
```bash
yarn lint
```
