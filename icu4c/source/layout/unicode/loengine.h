/*
 * @(#)loengine.h	1.0 00/12/11
 *
 * (C) Copyright IBM Corp. 1998, 1999, 2000 - All Rights Reserved
 *
 */

#ifndef __LOENGINE_H
#define __LOENGINE_H

#include "unicode/utypes.h"
#include "unicode/uscript.h"
#include "unicode/unistr.h"

#include "layout/LETypes.h"
#include "layout/LayoutEngine.h"

/**
 * This is a wrapper class designed to allow ICU clients to
 * use LayoutEngine in a way that is consistent with the rest
 * of ICU.
 *
 *(LayoutEngine was developed seperately from ICU and
 * the same source is used in non-ICU environments, so it cannot
 * be changed to match ICU coding conventions).
 *
 * This class is designed for clients who wish to use LayoutEngine
 * to layout complex text. If you need to subclass LayoutEngine,
 * you'll need to use the LayoutEngine interfaces directly.
 *
 * Basically, it creates an instance of LayoutEngine, stashes
 * it in fLayoutEngine, and uses it to implement the layout
 * functionality.
 *
 * Use the createInstance method to create an ICULayoutEngine. Use
 * delete to destroy it. The layoutChars method computes the glyphs
 * and positions, and saves them in the ICULayoutEngine object.
 * Use getGlyphs, getPositions and getCharIndices to retreive this
 * data.
 *
 * You'll also need an implementation of LEFontInstance for your platform.
 *
 * @see LayoutEngine.h
 * @see LEFontInstance.h
 */
class U_LAYOUT_API ICULayoutEngine
{
private:
	/**
	 * This is the table used to translate UScriptCode
	 * values to the script codes defined in LEScripts.h.
	 */
	static int32_t le_scriptCodes[];

	/**
	 * This method converts an UScriptCode value to
	 * a LayoutEngine script code as defined in LEScripts.
	 *
	 * @param script - the UScriptCode value
	 *
	 * @return the LayoutEngine script code.
	 *
	 * @see LEScripts.h
	 */
	static int32_t le_scriptCode(UScriptCode script);

	/**
	 * This holds the instance of LayoutEngine that does all
	 * the work.
	 */
	LayoutEngine *fLayoutEngine;

	/**
	 * This no argument constructor is private so that clients
	 * can't envoke it. Clients should use createInstance.
	 *
	 * @see createInstance
	 */
	ICULayoutEngine();

	/**
	 * The main constructor. It is defined as private to
	 * stop clients from invoking it. Clients should use
	 * createInstance.
	 *
	 * @param layoutEngine - the LayoutEngine that this instance wraps.
	 *
	 * @see createInstance
	 */
	ICULayoutEngine(LayoutEngine *layoutEngine);

public:

	/**
	 * The destructor. At least on Windows it needs to be
	 * virtual to ensure that it deletes the object from the
	 * same heap that createInstance will allocate it from. We
	 * don't know why this is...
	 *
	 * @see createInstance
	 */
	virtual ~ICULayoutEngine();

	/**
	 * This method computes the glyph, character index and position arrays
	 * for the input characters.
	 *
	 * @param chars - the input character context
	 * @param startOffset - the starting offset of the characters to process
	 * @param endOffset - the ending offset of the characters to process
	 * @param max - the number of characters in the input context
	 * @param rightToLeft - true if the characers are in a right to left directional run
	 * @param x - the initial X position
	 * @param y - the initial Y position
	 * @param success - output parameter set to an error code if the operation fails
	 *
	 * @return the number of glyphs in the glyph array
	 *
	 * Note; the glyph, character index and position array can be accessed
	 * using the getter method below.
	 */
    int32_t layoutChars(const UChar chars[],
						UTextOffset startOffset,
						UTextOffset endOffset,
						UTextOffset maxOffset,
						UBool rightToLeft,
						float x, float y,
						UErrorCode &success);


	/**
	 * This method computes the glyph, character index and position arrays
	 * for the input characters.
	 *
	 * @param str - the input character context
	 * @param startOffset - the starting offset of the characters to process
	 * @param endOffset - the ending offset of the characters to process
	 * @param rightToLeft - true if the characers are in a right to left directional run
	 * @param x - the initial X position
	 * @param y - the initial Y position
	 * @param success - output parameter set to an error code if the operation fails
	 *
	 * @return the number of glyphs in the glyph array
	 *
	 * Note; the glyph, character index and position array can be accessed
	 * using the getter method below.
	 */
	int32_t layoutString(const UnicodeString &str,
						 UTextOffset startOffset,
						 UTextOffset endOffset,
						 UBool rightToLeft,
						 float x, float y,
						 UErrorCode &success);

	/**
	 * This method returns the number of glyphs in the glyph array. Note
	 * that the number of glyphs will be greater than or equal to the number
	 * of characters used to create the LayoutEngine.
	 *
	 * @return the number of glyphs in the glyph array
	 */
	int32_t countGlyphs() const;

	/**
	 * This method copies the glyph array into a caller supplied array.
	 * The caller must ensure that the array is large enough to hold all
	 * the glyphs.
	 *
	 * @param glyphs - the destiniation glyph array
	 * @param success - output parameter set to an error code if the operation fails
	 */
    void getGlyphs(uint16_t glyphs[], UErrorCode &success);

	/**
	 * This method copies the character index array into a caller supplied array.
	 * The caller must ensure that the array is large enough to hold a character
	 * index for each glyph.
	 *
	 * @param charIndices - the destiniation character index array
	 * @param success - output parameter set to an error code if the operation fails
	 */
    void getCharIndices(int32_t charIndices[], UErrorCode &success);

	/**
	 * This method copies the character index array into a caller supplied array.
	 * The caller must ensure that the array is large enough to hold a character
	 * index for each glyph.
	 *
	 * @param charIndices - the destiniation character index array
	 * @param indexBase - an offset which will be added to each index
	 * @param success - output parameter set to an error code if the operation fails
	 */
    void getCharIndices(int32_t charIndices[], int32_t indexBase, UErrorCode &success);

	/**
	 * This method copies the position array into a caller supplied array.
	 * The caller must ensure that the array is large enough to hold an
	 * X and Y position for each glyph, plus an extra X and Y for the
	 * advance of the last glyph.
	 *
	 * @param glyphs - the destiniation position array
	 * @param success - output parameter set to an error code if the operation fails
	 */
    void getGlyphPositions(float positions[], UErrorCode &success);

	/**
	 * This method returns the X and Y position of the glyph at the
	 * given index.
	 *
	 * Input parameters:
	 * @param glyphIndex - the index of the glyph
	 *
	 * Output parameters:
	 * @param x - the glyph's X position
	 * @param y - the glyph's Y position
	 * @param success - output parameter set to an error code if the operation fails
	 *
	 */
    void getGlyphPosition(int32_t glyphIndex, float &x, float &y, UErrorCode &success);

	/**
	 * This method returns an ICULayoutEngine capable of laying out text
	 * in the given font, script and langauge.
	 *
	 * @param fontInstance - the font of the text
	 * @param scriptCode - the script of the text
	 * @param locale - used to determine the language of the text
	 * @param success - output parameter set to an error code if the operation fails
	 *
	 * @return an ICULayoutEngine which can layout text in the given font.
	 *
	 * NOTE: currently, locale is ignored...
	 *
	 * @see LEFontInstance
	 */
    static ICULayoutEngine *createInstance(const LEFontInstance *fontInstance,
										   UScriptCode script, Locale &locale,
										   UErrorCode &success);
};

inline ICULayoutEngine::ICULayoutEngine()
{
	// nothing at all...
}

inline ICULayoutEngine::ICULayoutEngine(LayoutEngine *layoutEngine)
	: fLayoutEngine(layoutEngine)
{
	// nothing else to do
}

inline ICULayoutEngine::~ICULayoutEngine()
{
	delete fLayoutEngine;
	fLayoutEngine = 0;
}

inline int32_t ICULayoutEngine::layoutChars(const UChar chars[],
											UTextOffset startOffset,
											UTextOffset endOffset,
											UTextOffset maxOffset,
											UBool rightToLeft,
											float x, float y,
											UErrorCode &success)
{
	// NOTE: call reset() so that clients can safely reuse
	fLayoutEngine->reset();
	return fLayoutEngine->layoutChars(chars,
									  startOffset,
									  endOffset - startOffset,
									  maxOffset,
									  rightToLeft,
									  x, y,
									  (LEErrorCode &) success);
}

inline int32_t ICULayoutEngine::layoutString(const UnicodeString &str,
											UTextOffset startOffset,
											UTextOffset endOffset,
											UBool rightToLeft,
											float x, float y,
											UErrorCode &success)
{
	int32_t glyphCount = 0;
	int32_t max = str.length();
	UChar *chars = new UChar[max];

	str.extract(0, max, chars);

	// NOTE: call reset() so that clients can safely reuse
	fLayoutEngine->reset();
	glyphCount = fLayoutEngine->layoutChars(chars,
									  startOffset,
									  endOffset - startOffset,
									  max,
									  rightToLeft,
									  x, y,
									  (LEErrorCode &) success);

	delete[] chars;

	return glyphCount;
}

inline int32_t ICULayoutEngine::countGlyphs() const
{
	return fLayoutEngine->getGlyphCount();
}

inline void ICULayoutEngine::getGlyphs(uint16_t glyphs[], UErrorCode &success)
{
	fLayoutEngine->getGlyphs(glyphs, (LEErrorCode &) success);
}

inline void ICULayoutEngine::getCharIndices(int32_t charIndices[], UErrorCode &success)
{
    fLayoutEngine->getCharIndices(charIndices, (LEErrorCode &) success);
}

inline void ICULayoutEngine::getCharIndices(int32_t charIndices[], int32_t indexBase, UErrorCode &success)
{
	fLayoutEngine->getCharIndices(charIndices, indexBase, (LEErrorCode &) success);
}

inline void ICULayoutEngine::getGlyphPositions(float positions[], UErrorCode &success)
{
    fLayoutEngine->getGlyphPositions(positions, (LEErrorCode &) success);
}

inline void ICULayoutEngine::getGlyphPosition(int32_t glyphIndex, float &x, float &y, UErrorCode &success)
{
    fLayoutEngine->getGlyphPosition(glyphIndex, x, y, (LEErrorCode &) success);
}

inline ICULayoutEngine *ICULayoutEngine::createInstance(const LEFontInstance *fontInstance,
														UScriptCode script,
														Locale &locale, UErrorCode &success)
{
	LayoutEngine *engine = LayoutEngine::layoutEngineFactory(fontInstance,
															 le_scriptCode(script),
															 0,
															 (LEErrorCode &) success);
	
	return new ICULayoutEngine(engine);
}

inline int32_t ICULayoutEngine::le_scriptCode(UScriptCode script)
{
	if (script < 0 || script >= USCRIPT_CODE_LIMIT) {
		return 0;
	}

	return le_scriptCodes[script];
}

#endif
