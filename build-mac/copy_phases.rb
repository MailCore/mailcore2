#!/usr/bin/env ruby

require 'xcodeproj'

dir = File.expand_path(File.dirname(__FILE__))
project_path = dir + '/mailcore2.xcodeproj'

project = Xcodeproj::Project.open(project_path)


def copy(project, src, dst)
    static_target = project.targets.detect { |t| t.name == src }
    framework_target = project.targets.detect { |t| t.name == dst }

    src_headers = static_target.copy_files_build_phases[0].files
    src_sources = static_target.source_build_phase.files

    dst_headers = framework_target.headers_build_phase
    dst_sources = framework_target.source_build_phase


    src_headers.each do |file|
        unless file.file_ref.real_path.to_s.end_with?("MailCore.h")
            new_file = dst_headers.add_file_reference(file.file_ref)
            new_file.settings = { "ATTRIBUTES" => ["Public"] }
        end
    end

    src_sources.each do |file|
        dst_sources.add_file_reference(file.file_ref)
    end
end

copy(project, "static mailcore2 osx", "mailcore osx")
copy(project, "static mailcore2 ios", "mailcore ios")

project.save
