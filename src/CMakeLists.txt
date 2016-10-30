file(GLOB cppfiles
    "*.cc"
    "*.cpp"
    "*.cxx"
)

add_executable(GemParticles
    ${cppfiles}
)

target_link_libraries(GemParticles
    ${ALL_LIBS}
)
