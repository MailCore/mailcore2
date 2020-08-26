Pod::Spec.new do |spec|
  spec.name         = "mailcore2-ios"
  spec.version      = "0.6.4"
  spec.summary      = "Mailcore 2 for iOS"
  spec.description  = "MailCore 2 provide a simple and asynchronous API to work with e-mail protocols IMAP, POP and SMTP. The API has been redesigned from ground up."
  spec.homepage     = "https://github.com/MailCore/mailcore2"
  spec.license      = { :type => "BSD", :file => "LICENSE" }
  spec.author       = "MailCore Authors"
  spec.platform     = :ios, "8.0"
  spec.source       = { :http => "https://d.etpan.org/mailcore2-deps/mailcore2-ios/mailcore2-ios-12.zip", :flatten => true }
  spec.header_dir   = "MailCore"
  spec.requires_arc = false
  spec.source_files = "include/MailCore/*.h"
  spec.public_header_files = "include/MailCore/*.h"
  spec.preserve_paths = "include/MailCore/*.h"
  spec.vendored_libraries = "lib/libMailCore-ios.a"
  spec.libraries = ["xml2", "iconv", "z", "c++", "resolv"]
  spec.prepare_command = "curl -O https://github.com/MailCore/mailcore2/raw/master/LICENSE"
end
