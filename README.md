## DuboPad

Crashpad integration for QT, as a library.

Licensed under BSD2.

## Useful documentation

 * https://bitbucket.org/chromiumembedded/cef/issues/1995/migrate-from-breakpad-to-crashpad
 * https://groups.google.com/a/chromium.org/forum/#!msg/crashpad-dev/NaB7PrfW04g/UACp0ahvCCYJ
 * https://documentation.backtrace.io/breakpad/#crashpad
 * https://chromium.googlesource.com/chromium/src/+/d413b2dcb54d523811d386f1ff4084f677a6d089/components/crash/app/crashpad_mac.mm
 * https://docs.sentry.io/cli/dif/#sentry-cli-dif
 * https://www.chromium.org/developers/decoding-crash-dumps
 * https://groups.google.com/forum/#!topic/google-breakpad-discuss/fierVnIAv1M
 * https://chromium-review.googlesource.com/c/breakpad/breakpad/+/1329752 (https://groups.google.com/forum/#!topic/google-breakpad-discuss/xwbJMvKSI-M)

## State of affairs

 * not clear what format is sent over the wire (sentry works, not electron mini server)
 * clarify what happens in term of debug/release building
 * implement and test for windows and linux
 * what happens is an app is relocated / moved after start, but before crash? (eg: handler path is invalid now?)

## Symbolification & Sentry

 * brew install getsentry/tools/sentry-cli
 * Generate pdb file dump_syms myappbinary > myappbinary.pbd
 * upload to sentry: sentry-cli --auth-token YOUR_AUTH_TOKEN upload-dif --org YOUR_ORG_SLUG --project YOUR_PROJECT_SLUG PATH_TO_DSYMS
 * rinse repeat last two steps for QtCore and any other library being linked

sentry-cli --auth-token your_authentication_token upload-dif --org dubo-dubon-duponey --project dubopad dubopad.pbd
