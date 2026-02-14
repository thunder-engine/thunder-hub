import qbs

Project {
    id: archive

    property stringList srcFiles: {
        var sources = [
            "src/*.c",
            "src/*.h"
        ];

        return sources;
    }

    property stringList incPaths: [
        "src"
    ]

    StaticLibrary {
        name: "7zip"
        files: archive.srcFiles
        Depends { name: "cpp" }
        Depends { name: "bundle" }
        bundle.isBundle: false

        cpp.defines: [ ]
        cpp.includePaths: archive.incPaths
        cpp.cxxLanguageVersion: "c++14"
        cpp.cxxStandardLibrary: "libc++"
    }
}
