# Change Log
This project adheres to [Semantic Versioning](http://semver.org/) and [ISO 8601](http://www.iso.org/iso/home/standards/iso8601.htm).

## [0.5.0] - 2016-08-07
### Added
- SSL support. Certificate verification is done as per [RFC 2818](https://tools.ietf.org/html/rfc2818).

## [0.4.1] - 2016-03-28
### Fixed
- `gen::valid_addr()` now works as intended, and should now match [RFC 1035](https://www.ietf.org/rfc/rfc1035.txt).

### Changes
- `gen::valid_addr()` now explains what makes an address invalid.

## [0.4.0] - 2016-03-26
### Added
- `WHOIS` and `WHOWAS` IRC commands.

### Changed
- `client::raw_cmd()` now throws an `std::invalid_argument` when `content` is empty.

## [0.3.0] - 2016-03-25
### Added
- `NOTICE`, `WHO`, `OPER`, and `MODE` IRC commands.

### Changed
- IRC commands will now throw `std::invalid_argument` when appropriate.

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

