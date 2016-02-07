set(SOURCES
		"srd.tex"
	)
set(INPUT_FILES
		"glossaire.tex"
	)
set(IMAGES_FILES
		"Diagrams/Activity1_Gameplay.png"
		"Diagrams/Collaboration1_SignIn.png"
		"Diagrams/Collaboration2_StartDuel.png"
		"Diagrams/Collaboration3_RoundDetails.png"
		"Diagrams/Model1_DecksManagement.png"
		"Diagrams/Model2_Classes.png"
		"Diagrams/UseCase1_Main.png"
	)

set(PDFLATEX_COMPILER "pdflatex")  # Alexis wanted to change this one

set(LATEX_OUTPUT_PATH "build/")

include(UseLATEX.cmake)  # see https://github.com/kmorel/UseLATEX for more informations

add_latex_document(
		"${SOURCES}"
		INPUTS "${INPUT_FILES}"
		#IMAGES "${IMAGES_FILES}"
		IMAGE_DIRS Diagrams/
		FORCE_PDF
		USE_GLOSSARY
	)

#add_custom_command(
#		TARGET pdf
#		POST_BUILD
#		COMMAND "cp build/srd.pdf ./srd.pdf"
#	)
add_custom_command(TARGET srd_pdf POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       build/srd.pdf
                       ./srd.pdf)
