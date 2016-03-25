# Change Log
This project adheres to [Semantic Versioning](http://semver.org/) and [ISO 8601](http://www.iso.org/iso/home/standards/iso8601.htm).

## [0.3.0] - 2016-03-25
### Added
- `NOTICE`, `WHO`, `OPER`, and `MODE` IRC commands.

### Changed
- IRC commands will now throw `std::invalid_argument()` when appropriate.

## [0.2.1] - 2016-03-06
### Added
- Address and port validation.

## [0.2.0] - 2016-02-01
### Added
- `AWAY`, `PART`, `TOPIC`, `NAMES`, `INVITE`, `KICK`, and `LIST` IRC commands.

### Changed
- Scope renamed from `libircppclient` to `irc`.

## [0.1.0] - 2016-01-30
### Added
- `NICK`, `JOIN`, `PRIVMSG`, and `QUIT` IRC commands.

### Fixed
- `irc::client` now handles pings.

